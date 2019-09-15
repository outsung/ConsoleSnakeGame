//------------------------------------------
Math.radians = function(degrees) {
  return degrees * Math.PI / 180;
};

//------------------------------------------
let gameplay = true;
let pause = {
  inventory : false,
  setting : false
}

let keyCode = {
  27 : false, //esc
  87 : false, //w
  65 : false, //a
  83 : false, //s
  68 : false //d
  //122 : false,
  //123 : false
}

let playerInfo = {
  position : {
    x : 0,
    y : 0
  },
  direction : 0,
  speed : 1
}


//--thread-------------------------------

//키 관련 함수 keyCode[] 중심
function keyPress(){
  if(gameplay === true){
    let down = event.keyCode;
    console.log(down);

    if (down in keyCode){
      keyCode[down] = true;
    }
    else{
      event.returnValue = false; // 브라우저 기능 키 무효화
    }
    //console.log(keyCode);
  }
}

function keyUp(){
  if(gameplay === true){
    let up = event.keyCode;
    //console.log(upKeyCode);

    if (up in keyCode){
      keyCode[up] = false;
    }
    //console.log(keyCode);
  }
}

function isKeyDown(key){
  if(gameplay === true){
    if(typeof(key) === "string"){
      //toUpperCase(소문자 -> 대문자)
      key = key.toUpperCase();
      //charCodeAt(아스키코드로 변환)
      key = key.charCodeAt(0);
      keyCode[key];
    }
    else return keyCode[key];
  }
}


//(2-3) (3-1)------
function resize(){
  let hUserScreen = document.getElementById("userScreen");

  console.log('Resizing...')

  //userScreen.width = window.innerWidth;
  //userScreen.height = window.innerHeight;

  hUserScreen.style.width = window.innerWidth + "px";
  hUserScreen.style.height = window.innerHeight + "px";
  if(gameplay === true){

    let hPlayer = document.getElementById("player");
    hPlayer.style.marginLeft = userScreen.width / 2 - 5 + "px";
    hPlayer.style.marginTop = userScreen.height / 2 - 5 + "px";
  }
};

//(2-2)
{
let hUserScreen = document.getElementById("userScreen");

//(2-2-1)
hUserScreen.addEventListener('click', function(event){
  //hUserScreen.requestPointerLock();
  if(pause.inventory || pause.setting) {

  }
  else {

  }
});

//(2-2-2)
hUserScreen.addEventListener('mousemove', function(event){
  //console.log("x : " + event.movementX + ", y : " + event.movementY);
  //mapRotate(event.movementX);
  if(pause.inventory || pause.setting) {

  }
  else {
    player.direction -= event.movementX * player.mouseSensitivity / 100;
    console.log(player.direction);
  }
});

}


// (1)---------------------------------------
{
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

}



}
// (2-1)-------------------------------------

// 2-1-0
{
let normalizeX = 0;
let normalizeY = 0;

let delta = {
  x : 0,
  y : 0
}

let directionDelta = 0; // dalta 각도
let ridian; // 위의 라디안값



if (isKeyDown("a")) normalizeX += -1;
if (isKeyDown("w")) normalizeY += -1;
if (isKeyDown("s")) normalizeY += 1;
if (isKeyDown("d")) normalizeX += 1;

// 대각선
if((normalizeX != 0) && (normalizeY != 0)){
   if(normalizeX == 1){
     if(normalizeY == 1) directionDelta = 45; // ↗
     else directionDelta = 315; // ↖
   }
   else{
     if(normalizeY == 1) directionDelta = 135; // ↘
     else directionDelta = 225; // ↙
   }
}
// 직선
else if((normalizeX != 0) || (normalizeY != 0)){
   if(normalizeY == 0){
     if(normalizeX == 1) directionDelta = 0; // ↑
     else directionDelta = 180; // ↓
   }
   else{
     if(normalizeY == 1) directionDelta = 90; // →
     else directionDelta = 270; // ←
   }
}
// 이동안함
else return 0;

ridian = Math.radians(playerInfo.direction - directionDelta);

// parseFloat (string -> float)
// toFixed (x) x자리만 표시
delta.x = parseFloat(Math.cos(ridian).toFixed(1));
delta.y = parseFloat(Math.sin(ridian).toFixed(1));

//이동
playerInfo.position.x += delta.x * playerInfo.speed;
playerInfo.position.y += delta.y * playerInfo.speed;

}

// 2-1-1, 2-1-2
{
if(isKeyDown("i")){
  pause.inventory = !pause.inventory;
}
// esc
else if(isKeyDown(27)){
  pause.setting = !pause.setting;
}
}


// (3)-----------------------------------------
// 3-2-1
{
let hMap = document.getElementById("map");
hMap.style.marginLeft = window.innerWidth - playerInfo.position.x;
hMap.style.marginTop = window.innerHeight - playerInfo.position.y;
}

//3-2-2
{


}
//--------------------------------------------


playerInfo = { // 계정의 설정
  Key : {
    up : "w",
    down : "s",
    left : "a",
    right : "d",
    inventory : "i",
    setting : "VK_esc"
  },
  mouse : {

  }
}

//--------------------------------------------

map = loadMap();
