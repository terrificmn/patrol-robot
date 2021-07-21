import cv2
import sys

#카메라 번호
#/dev/video0 = 0
# 이곳에 동영상 파일명의 위치를 넣어주면 동영상 재생으로 동작함
CAM_ID = 0

# 추적기능 상태
#얼굴 인식
TRACKING_STATE_CHECK = 0
#얼굴인식 위치를 기반으로 추적 기능 초기화
TRACKING_STATE_INIT  = 1
#추적 동작
TRACKING_STATE_ON    = 2

#OpenCV 버전 확인
(major_ver, minor_ver, subminor_ver) = (cv2.__version__).split('.')

if __name__ == '__main__' :
    #버전 출력
    print((cv2.__version__).split('.'))

    # 트레킹 함수 선택
    tracker_types = ['BOOSTING', 'MIL','KCF', 'TLD', 'MEDIANFLOW', 'GOTURN']
    # 기본 KCF(Kernelized Correlation Filters)가 속도가 빠르다.
    tracker_type = tracker_types[2]

    # OpenCV 서브 버전별 함수 호출명이 다르다.
    if int(minor_ver) < 3:
        #3.2 이하
        tracker = cv2.Tracker_create(tracker_type)
    else:
        #3.3 이상
        if tracker_type == 'BOOSTING':
            tracker = cv2.TrackerBoosting_create()
        if tracker_type == 'MIL':
            tracker = cv2.TrackerMIL_create()
        if tracker_type == 'KCF':
            tracker = cv2.TrackerKCF_create()
        if tracker_type == 'TLD':
            tracker = cv2.TrackerTLD_create()
        if tracker_type == 'MEDIANFLOW':
            tracker = cv2.TrackerMedianFlow_create()
        if tracker_type == 'GOTURN':
            tracker = cv2.TrackerGOTURN_create()

    #카메라 열기
    video = cv2.VideoCapture(CAM_ID)

    #카메라가 정상적으로 열리지 않았다면 프로그램 종료
    if not video.isOpened():
        print("Could not open video")
        sys.exit()

    #얼굴인식 함수 생성
    face_cascade = cv2.CascadeClassifier()
    #얼굴인식용 haar 불러오기
    #face_cascade.load('/usr/local/share/OpenCV/haarcascades/haarcascade_frontalface_default.xml')
    face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')
    #추적 상태 저장용 변수
    TrackingState = 0
    #추적 영역 저장용 변수
    TrackingROI = (0,0,0,0)

    #프로그램 시작
    while True:
        #카메라에서 1 frame 읽어오기
        ok, frame = video.read()
        #영상이 없다면 프로그램 종료를 위해 while 문 빠져나옴
        if not ok:
            break

        #추적 상태가 얼굴 인식이면 얼굴 인식 기능 동작
        #처음에 무조건 여기부터 들어옴
        if TrackingState == TRACKING_STATE_CHECK:
            #흑백 변경
            grayframe = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
            #히스토그램 평활화(재분할)
            grayframe = cv2.equalizeHist(grayframe)
            #얼굴 인식
            faces  = face_cascade.detectMultiScale(grayframe, 1.1, 5, 0, (30, 30))

            #얼굴이 1개라도 잡혔다면
            if len(faces) > 0:
                #얼굴 인식된 위치 및 크기 얻기
                x,y,w,h = faces[0]
                #인식된 위치및 크기를 TrackingROI에 저장
                TrackingROI = (x,y,w,h)
                #인식된 얼굴 표시 순식간에 지나가서 거의 볼수 없음(녹색)
                cv2.rectangle(frame,(x,y),(x+w,y+h),(0,255,0),3, 4, 0)
                #추적 상태를 추적 초기화로 변경
                TrackingState = TRACKING_STATE_INIT
                print('det w : %d ' % w + 'h : %d ' % h)

        #추적 초기화
        #얼굴이 인식되면 동작함
        elif TrackingState == TRACKING_STATE_INIT:
            #추적 함수 초기화
            #얼굴인식으로 가져온 위치와 크기를 함께 넣어준다.
            ok = tracker.init(frame, TrackingROI)
            if ok:
                #성공하였다면 추적 동작상태로 변경
                TrackingState = TRACKING_STATE_ON
                print('tracking init succeeded')
            else:
                #실패하였다면 얼굴 인식상태로 다시 돌아감
                TrackingState = TRACKING_STATE_CHECK
                print('tracking init failed')

        #추적 동작
        elif TrackingState == TRACKING_STATE_ON:
            #추적
            ok, TrackingROI = tracker.update(frame)
            if ok:
                #추적 성공했다면
                p1 = (int(TrackingROI[0]), int(TrackingROI[1]))
                p2 = (int(TrackingROI[0] + TrackingROI[2]), int(TrackingROI[1] + TrackingROI[3]))
                #화면에 박스로 표시 (파랑)
                cv2.rectangle(frame, p1, p2, (255,0,0), 2, 1)
                print('success x %d ' % (int(TrackingROI[0])) + 'y %d ' % (int(TrackingROI[1])) +
                        'w %d ' % (int(TrackingROI[2])) + 'h %d ' % (int(TrackingROI[3])))
            else:
                print('Tracking failed')

                TrackingState = TRACKING_STATE_CHECK

        #화면에 카메라 영상 표시
        #추적된 박스가 있으면 같이 표시됨
        cv2.imshow("Tracking", frame)

        #ESC키를 누르면 break로 종료
        k = cv2.waitKey(1) & 0xff
        if k == 27 : break
