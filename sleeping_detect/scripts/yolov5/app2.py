from typing import Any
import cv2
import tensorflow as tf
from tensorflow import keras
from tensorflow.keras import layers
import numpy as np
import winsound

frequency = 2500
duration = 1000
path = "haarcascade_frontalface_default.xml"
# eyes_roi = []
faceCascase = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')

cap = cv2.VideoCapture(0)

if not cap.isOpened():
    cap = cv2.VideoCapture(1)
if not cap.isOpened():
    raise IOError("Cannot open webcam")


eyes_roi = np.empty((224, 224, 3), float)
new_model = tf.keras.models.load_model('my_model_new.h5')

counter = 0 

while True:
    ret, frame = cap.read()
    # print(ret)
    eye_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_eye.xml')
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    #eyes_roi = np.empty((224, 224, 3), float)


    eyes = eye_cascade.detectMultiScale(gray, 1.1, 4)
    print(len(eyes))   # 눈으로 인식한 갯수
    #global eyes_roi
    for x,y,w,h in eyes:
        roi_gray = gray[y:y+h, x:x+w]
        roi_color = frame[y:y+h, x:x+w]
        cv2.rectangle(frame, (x,y), (x+w, y+h), (0,0,255),2)
        
        eyess = eye_cascade.detectMultiScale(roi_gray)

        if len(eyess)==0:
            print('Not eyes')

        else:
            #print(len(eyess))
            for (ex,ey,ew,eh) in eyess:
                eyes_roi = roi_color[ey:ey+eh , ex:ex+ew ]
                #print(type(eyes_roi))
                #print(eyes_roi.shape)

    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    faces =  faceCascase.detectMultiScale(gray, 1.1, 4)
    
    for (x,y,w,h) in faces:
        cv2.rectangle(frame, (x,y), (x+w, y+h), (0,255,0),2)

    font = cv2.FONT_HERSHEY_SIMPLEX


    final_image = cv2.resize(eyes_roi, (224,224))
    # print(eyes_roi)
    final_image = np.expand_dims(final_image,axis=0)
    final_image = final_image/255.0

    #new_model = tf.keras.models.load_model('my_model.h5') 

    prediction = new_model.predict(final_image)

    if prediction >0.3 :
        status = "Opened Eyes"
        cv2.putText(frame,status, (50,50), font, 3, (0,255,0), 2, cv2.LINE_4)
        x1,y1,w1,h1 = 0,0,175,75
        cv2.rectangle(frame, (x1,y1), (x1+w1, y1+h1), (0,0,0),-1)
        cv2.putText(frame,'Active', (x1+int(w1/10), y1+int(h1/2)), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0,255,0), 2)
    
    else:
        counter = counter +1
        status = "Closed Eyes"
        cv2.putText(frame,status, (50,50), font, 3, (0,0,255), 2, cv2.LINE_4)
        x,y,w,h = 0,0,175,75
        cv2.rectangle(frame, (x,y), (x+w, y+h), (0,0,255),2)
        
        if counter > 5:
            x1,y1,w1,h1 = 0,0,175,75
            cv2.rectangle(frame, (x1,y1), (x1+w1, y1+h1), (0,0,0),-1)
            cv2.putText(frame,'Sleep Alert', (x1+int(w1/10), y1+int(h1/2)), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0,255,0), 2)
            winsound.Beep(frequency, duration)
            counter = 0

    #print(status)
    #gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    #print(faceCascase.empty())

    #faces =  faceCascase.detectMultiScale(gray, 1.1, 4)

    # for (x,y,w,h) in faces:
    #     cv2.rectangle(frame, (x,y), (x+w, y+h), (0,255,0),2)

    # font = cv2.FONT_HERSHEY_SIMPLEX

    # cv2.putText(frame,status, (50,50), font, 3, (0,0,255), 2, cv2.LINE_4)

    cv2.imshow('b', frame)

    if cv2.waitKey(2) & 0xFF == ord('q'):
        break
    
cap.release()
cv2.destroyAllWindows()

