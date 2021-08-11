# 모바일 로봇 프로젝트
자율주행차를 만들고 있습니다. 3조와 협업 프로젝트 포함

3조의 인지 프로그램인 졸음 디텍팅과 
4조 제어 프로그램으로 ROS로 협업을 했습니다

우분투 18.04 LTS, ROS melodic 버전, 파이썬 3.6

<br/>

대표 패키지들 소개   
1. color_tracker, object_follower   
2. sleeping_detect, roll-out-car

<br/>

## color_tracker 패키지 
카메라를 통해서 원 모양의 특정 색 (녹색)을 추적하는 프로그램 입니다. 특정 색을 추적하면서 ROS를 이용해서 퍼블리싱을 합니다.

필요한 라이브러리 
```
python3 -m pip install imutils numpy rospkg PyYAML opencv-python
```
opencv를 못찾는 경우에는 pip을 업그레이드한다
```
pip3 install --upgrade pip
```
파이썬으로 되어 있으므로 카메라만 있다면 자체 실행 가능. 

ROS의 노드로 동작하게 하려면 catkin_make를 하고, 실행가능하게 권한을 바꿔줍니다.
```
cd ~/catkin_ws
catkin_make
```
그리고 나서 
```
cd src/patrol-robot/color_tracker/src
sudo chmod +x color_tracking.py
rospack profile
```
ROS 노드 실행
```
rosrun color_tracker color_tracking.py
```


<br/>

## object_follower 
color_tracker 패키지의 topic을 구독하여서 쫒아가는 프로그램 입니다. scout-mini 자동차에게 움직임을 
퍼블리싱 합니다.

필요한 패키지
- pointcloud_to_laserscan    
- realsense2_camera  

먼저 라이다 카메라 구동을 위해서 런치파일을 실행합니다
```
roslaunch object_follower point_to_laser.launch
```

그리고 follower 노드 실행
```
rosrun object_follower follower
```


<br/>

## sleeping_detect
졸음 디텍팅 프로그램 입니다. 카메라를 통해서 눈 깜빡임으로 졸음을 감지하며 눈을 오래 감고 있으면 
아두이노로 경고 LED을 표시하고 ROS를 통해 졸음 신호를 퍼블리싱 합니다.

필요한 라이브러리
```
python3 -m pip install torch pandas tqdm torchvision matplotlib seaborn searial
```
파이썬으로 되어 있으므로 카메라만 있다면 자체 실행 가능. 

ROS의 노드로 동작하게 하려면 catkin_make를 하고, 파이썬 파일을 실행가능하게 권한을 바꿔주면 됩니다.
```
cd ~/catkin_ws
catkin_make
```
그리고 나서 
```
cd src/patrol-robot/sleeping_detect/scripts/yolov5 
sudo chmod +x detect.py
rospack profile
```

이제 파이썬 스크립트를 실행할 수 있지만 그 전에 arduino를 실행해야합니다.  
sleeping_detect/arduino_code/serial_for_detection.ino 을 실행해줍니다.  

(arduino 와 아두이노 IDE인 스케치북가 필요합니다.)  
[다운로드](https://www.arduino.cc/en/software)  
[설치 및 셋업 참고하기](http://wiki.ros.org/rosserial_arduino/Tutorials/Arduino%20IDE%20Setup)  

<br/>

이제 파이썬 스크립트 실행
```
rosrun sleeping_detect detect.py
```


<br/>

## roll-out-car
slepping_detect 프로그램에서 보내온 topic을 받아서 scout-mini 자동차를 제어하는 프로그램 입니다.
기본으로 사각형 모양으로 움직이다가 졸음 신호가 오면 자동차가 멈추게 됩니다.

런치파일로 실행
```
roslaunch roll-out-car rollout.launch
```

<br/>

<br/>

<br/>

___

## 프로젝트 진행 중 git 사용법 입니다. 잘 지켜주세요~
깃에서 **브랜치**를 사용해서 작업을 해주세요!

먼저 오른쪽 상단의 Fork버튼을 눌러서 본인의 깃허브로 가져옵니다. 그리고 깃 클론을 해줍니다

```
$ git clone 본인의계정/patrol-robot
```

그리고 브랜치를 만들어주세요. 브랜치명 짓기는 아래처럼 해주세요.  
이름 이니셜과 하는 부분의 파트를 만들어 주시면 됩니다. (이니셜-개발파트이름)
예: sm-mapping
```
$ git branch 이니셜-개발파트이름
```

사실 브랜치명은 알아볼 수 있게 편하게 만들어도 상관없습니다.

만들어진 브랜치로 이동해 줍니다. sm-mapping 이라고 가정한다면
```
$ git checkout sm-mapping
```
브랜치가 바뀌었다고 나오면 정상
```
Switched to branch 'sm-mapping'
```

현재 브랜치 위치 확인하기
```
$ git branch 
```

현재 위치한 브랜치 앞에 * 표시가 붙고, 녹색으로 표시가 됩니다.
```
  main
* sm-mapping
```

이제 branch가 바뀌었으므로   

먼저 본인이 작업할 pkg 디렉토리를 하나 만들어 줍니다. 또는 가지고 있는 패키지 디렉토리를 복사/이동 시킵니다.  
그리고 그 안에서 작업 하시고, 여기에서 작업을 하고 add, commit, push 순서로 작업을 하시면 됩니다.

깃에 파일 추가해서 stage 로 인식시키기 (깃이 파일을 추적합니다)
```
$ git add [filename]
```
수정사항을 커밋~ 
```
$ git commit -m "a message for commit"
```

최종적으로 push로 remote 깃 허브에 올립니다.   
주의할점은 origin main으로 직접 **푸시하시면 안됩니다!** 본인의 브랜치명으로 푸쉬를 해줍니다  
origin [본인의브랜치명]
```
$ git push -u origin sm-mapping
```

**하루 마감** 일때 또는 적당한 시기(?)에 깃허브에서 pull request를 해주면 됩니다.
자신의 깃허브 사이트로 오면 pull request를 하라고 나오는데 적당히 클릭 클릭 해주면 됩니다.

![pullrequest1](img/open_pull_request1.png)

![pullrequest2](img/open_pull_request2.png)
  
<br/>

**OR**  
아래 내용은 깃허브 사이트에 말고 또는 CLI로 터미널에서 하려면 아래의 내용을 참고하세요   
먼저 main 브랜치로 이동해 줍니다.    
본인의 브랜치에서 마지막으로 commit한 내용을 push를 한 다음에 

```
$ git checkout main
```
그리고 나서, git merge [본인의 브랜치] 를 합니다
```
$ git merge sm-mapping
```
메인에서 별 다른 변경이 없었다면 fast-farward 방식으로 합병이 됩니다.  
아래는 예시 입니다.
```
Updating ce9b99f..34619a4
Fast-forward
 test2_dir/test2.txt | 1 +
 test_dir/hello.txt  | 1 +
 2 files changed, 2 insertions(+)
 create mode 100644 test2_dir/test2.txt
 create mode 100644 test_dir/hello.txt
```

그리고 상태 확인
```
$ git status
```
를 해보면
```
On branch main
Your branch is ahead of 'origin/main' by 2 commits.
  (use "git push" to publish your local commits)
```
main브런치가 2개의 commit이 앞서 있다고 나옵니다. 즉 sm-mapping에서 push한 내용이 (2번의 commit) 을 이미 한 후에  
main 브랜치와 합쳐졌기 때문에 기존의 main브랜치 입장에서는 처음을 클론을 받았을 때의 
상태이기 때문에 commit이 없는 상태  
그래서 2개의 커밋이 앞서 있다고 나오는 것임. 그래서 나온것 처럼 git push를 해주면 됩니다.
그러면 깃허브에 (remote main)으로 업데이트가 되는 것

```
$ git push
```
하면 깃허브에도 최종적으로 반영이 되게 됩니다. 조금 복잡할 수 있지만 익숙해지면 편해질 듯(?)도 합니다.

<br/>

___

## 추가로 유용한 것 깃 명령어들, 참고하세요
이메일과 아이디가 등록이 안되어 있을 때 
```
$git config --global user.name "YourID"
$git config --global user.email "youremail@domain.com"
```
원하지 않는 디렉토리/ 파일 제외하려면 깃이 있는 디렉토리에 .gitignore 파일을 만들고, 내용에 디렉토리 명을 넣어준다
```
.vscode
data/image
```

브랜치를 만들면서 바로 브랜치 변경하기
```
$ git checkout -b 브랜치명
```

브랜치 삭제하기
```
$ git branch -d 브랜치명
```

커밋한 것 취소하기. 커밋 바로 전 상태로 되돌리기
```
$ git reset --soft HEAD~1
```

add 했던 것 취소하기 
```
$ git reset HEAD file명
```

깃의 메인 브랜치가 통합되서 업데이트가 되어서 그 최신 main브랜치 내용으로 업데이트하고 싶을 때 
```
$ git checkout main
$ git rebase origin/main
```

git status를 했을 때 한글파일명이 354\213\234\354\236\221.txt 이런식으로 나올 때 설정해주기
```
$ git config --global core.quotepath false
```

윈도우에서 작업을 한다면 줄바꿈 충돌이 안나게 설정해주기 (윈도우 only)
```
$ git config --global core.autocrlf true
```

