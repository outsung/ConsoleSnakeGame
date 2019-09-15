

//------------내장 함수-------------------

//사이즈 변경하면 실행하는 함수
function resize() {
  if(oMain.oGame.vStart === true){

    let hUserScreen = document.getElementById("userScreen");

    console.log('Resizing...')

    //userScreen.width = window.innerWidth;
    //userScreen.height = window.innerHeight;

    hUserScreen.style.width = window.innerWidth + "px";
    hUserScreen.style.height = window.innerHeight + "px";



    //let hPlayer = document.getElementById("player");
    //hPlayer.style.marginLeft = userScreen.width / 2 - 5 + "px";
    //hPlayer.style.marginTop = userScreen.height / 2 - 5 + "px";
  }
};

//키 입력 시 실행하는 함수
function keyPress(){
  if(oMain.oPlay.vStart === true){
    let down = event.keyCode;
    console.log(downKeyCode);

    if (down in oMain.oPlay.aKeyCode){
      oMain.oPlay.aKeyCode[down] = true;
    }
    else{
      event.returnValue = false; // 브라우저 기능 키 무효화
    }
    //console.log(keyCode);
  }
}

function keyUp(){
  if(oMain.oPlay.vStart === true){
    let up = event.keyCode;
    //console.log(upKeyCode);

    if (up in oMain.oPlay.aKeyCode){
      oMain.oPlay.aKeyCode[up] = false;
    }
    //console.log(keyCode);
  }
}


function isKeyDown(key){
  if(oMain.oPlay.vStart === true) return oMain.oPlay.aKeyCode[key];
  else console.log("iskeydown error!!");
}

//----------------------------------------



function getRandomInt(min, max){
  return Math.floor(Math.random() * (max - min)) + min;
}

function getRandomBool(threshold){
    return threshold >= getRandomInt(1,101);
}


function playerPositionDelta(theta){

  let ridian = Math.radians(theta);

  let vPlayerPositionDelta = {
    deltaX : parseFloat(Math.cos(ridian).toFixed(1)),
    deltaY : parseFloat(Math.sin(ridian).toFixed(1))
  };

  return vPlayerPositionDelta;
}

function playerMove(){

  let directionDelta = 0;

  let normalizeX = 0;
  let normalizeY = 0;

  if (isKeyDown(65)) // A
  {
    normalizeX += -1;
  }
  if (isKeyDown(87)) // W
  {
    normalizeY += -1;
  }
  if (isKeyDown(83)) // S
  {
    normalizeY += 1;
  }
  if (isKeyDown(68)) // D
  {
    normalizeX += 1;
  }


  // 대각선 이동
  if((normalizeX != 0) && (normalizeY != 0))
  {
    if(normalizeX == 1){
      if(normalizeY == 1) directionDelta = 45; // ↗
      else directionDelta = 315; // ↖
    }
    else{
      if(normalizeY == 1) directionDelta = 135; // ↘
      else directionDelta = 225; // ↙
    }
  }
  // 직선 이동
  else if((normalizeX != 0) || (normalizeY != 0))
  {
    if(normalizeY == 0){
      if(normalizeX == 1) directionDelta = 0; // ↑
      else directionDelta = 180; // ↓
    }
    else{
      if(normalizeY == 1) directionDelta = 90; // →
      else directionDelta = 270; // ←
    }
  }
  // 이동 안함
  else return 0;

  let vPlayerPositionDelta = playerPositionDelta(player.direction - directionDelta);


  let vPlayerPositionDeltaPixel = getPixel(player.position.x + vPlayerPositionDelta.deltaX * player.speed,
                                            player.position.y - vPlayerPositionDelta.deltaY * player.speed);

  let vPlayerPositionDeltaSlope = vPlayerPositionDelta.deltaX * player.speed /
                                vPlayerPositionDelta.deltaY * player.speed;
  console.log(vPlayerPositionDeltaPixel);
  if(vPlayerPositionDeltaPixel[0] == 255 &&
    vPlayerPositionDeltaPixel[1] == 255 &&
    vPlayerPositionDeltaPixel[2] == 255 &&
    vPlayerPositionDeltaPixel[3] == 255){

    console.log("NOTWALL!!");
    player.position.x += vPlayerPositionDelta.deltaX * player.speed;
    player.position.y -= vPlayerPositionDelta.deltaY * player.speed;

  }
  else{/*
    let wall = {
      x : player.position.x + vPlayerPositionDelta.deltaX * player.speed,
      y : player.position.y - vPlayerPositionDelta.deltaY * player.speed
    }
    while(true)
    {
        if(vPlayerPositionDeltaSlope)
    }
    //console.log("wall!");
    //console.log("wall x : " + (player.position.x + vPlayerPositionDelta.deltaX * player.speed));
    //console.log("wall y : " + (player.position.y - vPlayerPositionDelta.deltaY * player.speed));
    */
  }
}





//--------------상황------------

function fGame(){ // 게임 시작
    // 마우스 위치?
    this.vStart = false;

    Math.radians = function(degrees) {
        return degrees * Math.PI / 180;
    };

}


function fPlay(){ // 게임 플레이

    // 마우스 고정 & 마우스
    this.vStart = false;

    //https://blog.outsider.ne.kr/322
    this.aKeyCode = {
    27 : false,
    87 : false,
    65 : false,
    83 : false,
    68 : false,
    122 : false,
    123 : false
    };

    this.userInfo = {
      //관전 가능~
      public : {
        // 위치
        position : {
          x : 0,
          y : 0
        },
        // 보는 방향
        direction : 0
      },
      //관전 불가능
      private : {
        // 스피드 ->
        speed : 1,
        //마우스 감도 ->
        mouseSensitivity : 10
      }
    }


}

function fUi(){ // 게임 ui

    // 마우스 not 고정 &
    this.vStart = false;
}

//-------------------------------


//----------맵-----------------
function fMap(width, height, initV){
    this.width = width;
    this.height = height;
    this.initV = initV;

    this.blockSize = 20;


    // 기본값 생성
    this.getA = function(){
        let j = 0;
        let i = 0;
        let res = new Array;
        let temp = new Array;

        while(j < this.height + 2)
        {
            i = 0;
            temp = new Array;
            while(i < this.width + 2)
            {
                if(i === 0 || i === this.width + 1 ||
                  j === 0 || j === this.height + 1){
                    //console.log("i = "+ i +" j = " + j);
                    temp.push(false);
                    i++;
                }
                else{
                    temp.push(getRandomBool(this.initV));
                    i++;
                }
            }
            //console.log(temp);
            res.push(temp);
            j++;
        }
        console.log("x "+this.width+" y "+this.height+" setting..");

        // 크기 조정
        /*
        let hMap = document.getElementById("map");
        hMap.style.width = (this.width + 2) * this.blockSize + "px";
        hMap.style.height = (this.height + 2) * this.blockSize + "px";
        console.log("x = " + (this.width + 2) * this.blockSize +
                    " y = " + (this.height + 2) * this.blockSize);
        */
        let hMap = document.getElementById("map");
        hMap.width = (this.width + 2) * this.blockSize;
        hMap.height = (this.height + 2) * this.blockSize;
        //console.log("_____________"+res);
        return res;
    };


    this.aMap = this.getA();

    // 세포자동화
    this.fSellularAutomata = function(rule){

        let aB = [false, false, false, false, false, false, false, false, false, false];
        let aS = [false, false, false, false, false, false, false, false, false, false];

        let fGetNeighbors = function(aMap,x,y){
            /*
            [1][0][1]
            [0][?][0]
            [0][1][0]
            */

            let vNeighbors = {
                alive : 0,
                dead : 0
            }

            let i = x-1;
            let j = y-1;

            while(j <= y + 1)
            {
                i = x-1;
                while(i <= x + 1)
                {
                    if(j === y && i === x){
                        i++;
                        continue;
                    }
                    else{
                        if(aMap[j][i]) vNeighbors.alive++;
                        else vNeighbors.dead++;
                    }
                    i++;
                }
                j++;
            }

            //console.log(vNeighbors.dead + vNeighbors.alive);
            if(vNeighbors.dead + vNeighbors.alive !== 8) console.log("getNei.. error roop!!");
            else return vNeighbors;
        }


        let sRuleB = rule.split("/")[0].split("");
        let sRuleS = rule.split("/")[0].split("");

        //console.log(sRuleB);
        for(let i in sRuleB)
        {
            //console.log(sRuleB[i]);
            if(typeof(sRuleB[i] *= 1) === "number") aB[sRuleB[i]] = true;
            else{
                if(sRuleB[i] !== "B"){
                    console.log("fMapSellularAutomata-[B]error-!");
                    return -1;
                }
            }
        }
        for(let i in sRuleS)
        {
            if(typeof(sRuleS[i] *= 1) === "number") aS[sRuleS[i]] = true;
            else{
                if(sRuleS[i] !== "S"){
                    console.log("fMapSellularAutomata-[S]error-!");
                    return -1;
                }
            }
        }

        console.log(sRuleB);
        //start
        let j = 1;
        let i = 1;
        let vNeighbors = {};

        while(j < this.height + 1)
        {
            i = 1;
            while(i < this.width + 1)
            {
                vNeighbors = fGetNeighbors(this.aMap,i,j);
                //console.log(vNeighbors);
                if(this.aMap[j][i] === true){ // Alive
                    if(aS[vNeighbors.alive] === true);
                    else this.aMap[j][i] = false;
                }
                else{ // Dead
                    if(aB[vNeighbors.alive] === true){ // B
                        this.aMap[j][i] = true;
                    }
                }


                i++;
            }
            //console.log(temp);
            j++;
        }

    }


    this.fDraw = function(){

        console.log("B");
        // 맵 그리기

        //-----------블럭-----------
        let alive = function(blockSize,x,y){
            x *= blockSize;
            y *= blockSize;

            const hMap = document.getElementById("map");
            const hDraw = hMap.getContext("2d");


            //const width = 10;
            //const height = 10;
            const style = "rgb(0,128,0)";


            hDraw.fillStyle = style;

            hDraw.fillRect (x, y, blockSize, blockSize);
            //console.log("alive draw");

        }
        let dead = function(blockSize,x,y){
            x *= blockSize;
            y *= blockSize;

            let hMap = document.getElementById("map");
            if(hMap.getContext){
              let hDraw = hMap.getContext("2d");


              //const width = 10;
              //const height = 10;
              const style = "rgb(80,188,223)";


              hDraw.fillStyle = style;
              //console.log("x = " + x);
              //console.log("y = " + y);
              //console.log("y = " + this.blockSize);
              //hDraw.fillRect(0, 0, hMap.width, hMap.height);
              hDraw.fillRect (x, y, blockSize, blockSize);

            }
            else console.log("에바인 에러");

        }
        //-------------------------

        //console.log(this.aMap);
        for(let j in this.aMap){
            //console.log("j = "+j);
            for(let i in this.aMap[0]){
                //console.log("i = "+i);
                if(this.aMap[j][i] === true){
                  //console.log("alive draw");
                  alive(this.blockSize, i, j);
                }
                else{
                  //console.log("dead draw");
                  dead(this.blockSize, i, j);
                }
            }
        }


    }


    this.fRotate = function(deltaX){
      if(oMain.oPlay.vStart === true){
        oMain.oPlay.userInfo.public.direction -= deltaX * player.mouseSensitivity / 100;
        console.log(player.direction);
      }
    }
}
//-------------------------------


//----------------------------------

//----------------------------------



//-------------------main-----------

function main(){

    this.oGame = new fGame();
    //console.log(this.oGame);

    this.oPlay = new fPlay();

    //let oUi = new fUi();

    this.oMap = new fMap(100,100,50);


    //this.oMap.fDrawMap();
    //console.log("AAAAA");

    //vMap.fMapSellularAutomata("B23/S45");

    this.updata = function(){
      let iRule = "B23/S45"
      let delay = 1;
      resize();

      //oMain.oMap.fMapSellularAutomata("B23/S45");

      this.oMap.fDrawMap();

      setInterval(maaaap(),10);
    }


}

function maaaap(){
  /*
  console.log("z");
  if((isKeyDown(65) === true) && (delay === 1)){
    console.log("A");
    this.oMap.fMapSellularAutomata(iRule);
    this.oMap.fDrawMap();
    delay = 0;
  }
  if((isKeyDown(65) !== true) && (delay === 1)){
    delay = 1;
  }
  */
}


//----------------------------------


//--------------------실행-----------
let oMain = new main();
//console.log("------------" + oMain.oPlay.vStart);
oMain.updata();

//main();
//----------------------------------




{
  게임 흐름
  rander -> updata

  ---ui---
  게임 시작

  메인 화면 -- {
                게임 타이틀... (게임을 대표 하는 이름,
                              디자인적 요소가 대부분)
                [ 아이디 ]... (유저를 대표하는 이름(아이디),
                              본인명의 당 한개의 계정)
                게임 플레이하기... (방을 찾고 (난입 가능), 플레이하기)
                  ㄴ로딩 화면.. (로딩화면)
              }

  게임 플레이 -- {
                  캐릭터... (계정 당 하나,
                           간단한 꾸미기 가능,
                           부가 능력 보류)
                   ㄴhand..(아이템 하나 들 수 있음,
                           인벤토리에서 들기, public)
                   ㄴ가방..(public, 들어 있는 양에 따라 모양 변화)
                  맵... (방 하나당 맵 하나, 자동 맵 생성)
                   ㄴ환경..
                    B-바다, 땅
                   ㄴ시간..
                    B-낮, 밤
                   ㄴ날씨..
                    B-맑음, 비
                  pause... (마우스 락 off,
                            포커스 아웃인 공간 어둡게)
                   ㄴ인벤토리..(가방, 보는 모션 (public))
                   ㄴ설정(ESC)..()
                    -입력 (키보드, 마우스)
                    -나가기 (방 나가기, 다시 접속 불가능)
                  enemy...()
                   ㄴ지상.. ()
                }

  ---founction---
  메인 화면 -- {
                아이디 확인...
                방 매칭...
              }
  게임 플레이 -- {
                  init...(1)
                    맵 생성
                    +플레이어 배치
                  render...(2)
                    Is Key Down?..(2-1)
                      +move [w],[a],[s],[d] (2-1-0)
                      +inventory [i] (2-1-1)
                      +set [esc] (2-1-2)
                    mouse input..(2-2)
                      +click(2-2-1)
                      +move(2-2-2)
                    resize..(2-3)
                    map..(2-4)
                      object(2-4-1)
                        +interaction, +adaptive
                  updata...(3)
                    char..(3-1) 중앙 위치
                      body(3-1-1)
                      head(3-1-2)
                      hand(3-1-3)
                      bag(3-1-4)
                      other*(3-1-5)
                    map..(3-2)
                      background(3-2-1)
                        margin, rotate, night
                      object(3-2-2)
                        interaction(3-2-2-1)
                        adaptive(3-2-2-2)
                    enemy..(3-4)
                      position
                      focus(user position)
                        A*(최단거리 알고리즘)
                      move(speed)
                      attack



                }

  ---server---
  메인 화면 -- {

              }
  게임 플레이 -- {

                }



}
