from typing import Any
import cv2
import tensorflow as tf
from tensorflow import keras
from tensorflow.keras import layers
import numpy as np
#import winsound
import time
import os
import serial



# duration = 1  # seconds
# freq = 440  # Hz
# 우분투 비프,   안됨..
# def beep():
#     print("\a")

# def playsound(frequency,duration):
#         #apt-get install beep
#         os.system('beep -f %s -l %s' % (frequency,duration))

# winsound용
# frequency = 2500
# duration = 1000

ser = serial.Serial('/dev/ttyUSB0', 9600)
#path = "haarcascade_frontalface_default.xml"

faceCascase = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')
eye_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_eye_tree_eyeglasses.xml')


# 카메라 잘 불러왔는지 확인
cap = cv2.VideoCapture(0)

if not cap.isOpened():
    cap = cv2.VideoCapture(1)
if not cap.isOpened():
    raise IOError("Cannot open webcam")


eyes_roi = np.empty((224, 224, 3), float)

# 눈동자 감지 모델 불러오기.
new_model = tf.keras.models.load_model('my_model_new.h5')

last_open_time = time.time()   # 프로그램 돌때 시간


# 캠 돌아가고.
while True:
    
    ret, frame = cap.read()
    # print(ret)
    
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    #  얼굴 검출하고 

    faces =  faceCascase.detectMultiScale(gray, 1.1, 4)
    
    prediction_list=[]  #예측결과 리스트
    for (x,y,w,h) in faces:
        cv2.rectangle(frame, (x,y), (x+w, y+h), (0,255,0),2)
        roi_gray = gray[y:y+h, x:x+w]
        roi_color = frame[y:y+h, x:x+w]
        
        #cv2.imwrite('face.jpg')
        # 얼굴 검출한 부분에서 눈 검출하여 검출한 눈이 뜬건지 감은건지 예측.
        eyes = eye_cascade.detectMultiScale(roi_gray, 1.1, 4)
        #print(len(eyes))   # 눈으로 인식한 갯수

        if len(eyes) == 0 :
            print("Not eyes")
    
        else:
            for ex,ey,ew,eh in eyes:
                
                cv2.rectangle(roi_color, (ex,ey), (ex+ew, ey+eh), (0,0,255),2)    
                eyes_roi = roi_color[ey:ey+eh , ex:ex+ew ]
                final_image = cv2.resize(eyes_roi, (224,224))   
                final_image = np.expand_dims(final_image,axis=0)
                final_image = final_image/255.0
                prediction = new_model.predict(final_image)
                prediction_list.append(prediction)
                
               
            # print(prediction_list[0])
    
    # 조건을 주고 두 눈이 감겼을때 , close 하고, 나머지는 그냥 open처리
    print(len(prediction_list)) 

    font = cv2.FONT_HERSHEY_SIMPLEX

    if len(prediction_list) == 2 :
                
        if prediction_list[0] < 0.1 and prediction_list[1] < 0.1 :
            
            #   시간 계싼..
            now = time.time()

            delta_time = now - last_open_time
            #print(type(delta_time))
            print(delta_time)
            #counter = counter +1
            status = "Closed Eyes"
            # 문자열이 표시될 위치. 문자열의 bottom-left corner점  putText(50,50)
            cv2.putText(frame,status, (150,100), font, 2, (0,0,255), 2, cv2.LINE_4)
            x,y,w,h = 0,0,175,75
            cv2.rectangle(frame, (x,y), (x+w, y+h), (0,0,255),2)
            
            if delta_time >= 5:
                x1,y1,w1,h1 = 0,0,175,75
                cv2.rectangle(frame, (x1,y1), (x1+w1, y1+h1), (0,0,0),-1)
                cv2.putText(frame,'Sleep Alert', (x1+int(w1/10), y1+int(h1/2)), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0,255,0), 2)
            
                val='1'
                val = val.encode('utf-8')
                ser.write(val)
                print("LED ON")
                time.sleep(0.5)
                  
                          
                #winsound.Beep(frequency, duration)
                #last_open_time = now   # 계속 감겨있을때는 리셋 안함.
                #os.system('play -nq -t alsa synth {} sine {}'.format(duration, frequency))
                #beep()
                #playsound(frequency,duration)
                  

        else :
            last_open_time = time.time()   # 현재시간 초기화
            #counter=0
            status = "Opened Eyes"
            cv2.putText(frame,status, (150,100), font, 2, (0,255,0), 2, cv2.LINE_4)
            x1,y1,w1,h1 = 0,0,175,75
            cv2.rectangle(frame, (x1,y1), (x1+w1, y1+h1), (0,0,0),-1)
            cv2.putText(frame,'Active', (x1+int(w1/10), y1+int(h1/2)), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0,255,0), 2)    

    else :
        pass        


    cv2.imshow('cam Window', frame)

    if cv2.waitKey(2) & 0xFF == ord('q'):
        break
    
cap.release()
cv2.destroyAllWindows()

