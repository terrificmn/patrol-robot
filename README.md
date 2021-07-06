# 경비 로봇 프로젝트
Souct-Mini 자율주행차를 만들고 있습니다.

README 파일은 업데이트 예정입니다...

<br/>

## 깃 사용법 입니다. 잘 지켜주세요~
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

이제 branch가 바뀌었으므로 여기에서 작업을 하고 add, commit, push 순서로 작업을 하시면 됩니다.

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

