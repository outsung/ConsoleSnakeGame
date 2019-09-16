//------------------------------------------
Math.radians = function(degrees) {
  return degrees * Math.PI / 180;
};


function getRandomInt(min, max){
  return Math.floor(Math.random() * (max - min)) + min;
}

function getRandomBool(threshold){
    return threshold >= getRandomInt(1,101);
}

//------------------------------------------
let gameplay = true;
let check = {
  inventory : false,
  setting : false,
  mapChange : true
}

let keyCode = {
  27 : false, //esc
  87 : false, //w
  65 : false, //a
  83 : false, //s
  68 : false, //d
  77 : false //m
  //122 : false,
  //123 : false
}

let playerInfo = {
  position : {
    x : 0,
    y : 0
  },
  direction : 0,
  speed : 1,
  mouseSensitivity : 10
}

//나중에 scale 변경 할 때 같이 변경됨

let map = new Array;

let mapInfo = {
  width : 200,
  height : 200,
  blockSize : 20,
  initV : 50
}

// true = 가능
// false = 불가능
let delay = {
  mapChange : true,
  update : 10,
  attack : true
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


//define
//(2-2)
{
let hUserScreen = document.getElementById("userScreen");

//(2-2-1)
hUserScreen.addEventListener('click', function(event){
  //hUserScreen.requestPointerLock();
  if(check.inventory || check.setting) {

  }
  else {

  }
});

//(2-2-2)
hUserScreen.addEventListener('mousemove', function(event){
  //console.log("x : " + event.movementX + ", y : " + event.movementY);
  //mapRotate(event.movementX);
  if(check.inventory || check.setting) {

  }
  else {
    playerInfo.direction -= event.movementX * playerInfo.mouseSensitivity / 100;
    //console.log(player.direction);
  }
});

}


// (1)---------------------------------------
//getMap
{
let j = 0;
let i = 0;
let temp = new Array;

while(j < mapInfo.height + 2)
{
    i = 0;
    temp = new Array;
    while(i < mapInfo.width + 2)
    {
        if(i === 0 || i === mapInfo.width + 1 ||
          j === 0 || j === mapInfo.height + 1){
            //console.log("i = "+ i +" j = " + j);
            temp.push(false);
            i++;
        }
        else{
            temp.push(getRandomBool(mapInfo.initV));
            i++;
        }
    }
    //console.log(temp);
    map.push(temp);
    j++;
}
console.log("x "+mapInfo.width+" y "+mapInfo.height+" setting..");

// 크기조정
let hMap = document.getElementById("map");
hMap.width = (mapInfo.width + 2) * mapInfo.blockSize;
hMap.height = (mapInfo.height + 2) * mapInfo.blockSize;
//console.log("_____________"+res);

// player position change
playerInfo.position.x = getRandomInt(0,(mapInfo.width + 2) * mapInfo.blockSize);
playerInfo.position.y = getRandomInt(0,(mapInfo.height + 2) * mapInfo.blockSize);
}

let startTime = new Date().getTime();
while(new Date().getTime() - startTime > 10){
  console.log("z  ");
  // (2-1)----------render--------------------

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
  else {

  }

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
    check.inventory = !check.inventory;
    // key code reset
  }
  // esc
  else if(isKeyDown(27)){
    check.setting = !check.setting;
    // key code reset
  }

  if(isKeyDown("m") && delay.mapChange){
    delay.mapChange = false;
    let rule = "B23/S45"

    let aB = [false, false, false, false, false, false, false, false, false, false];
    let aS = [false, false, false, false, false, false, false, false, false, false];

    let fGetNeighbors = function(x,y){
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
                    if(map[j][i]) vNeighbors.alive++;
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
            }
        }
    }
    for(let i in sRuleS)
    {
        if(typeof(sRuleS[i] *= 1) === "number") aS[sRuleS[i]] = true;
        else{
            if(sRuleS[i] !== "S"){
                console.log("fMapSellularAutomata-[S]error-!");
            }
        }
    }

    console.log(sRuleB);
    //start
    let j = 1;
    let i = 1;
    let vNeighbors = {};

    while(j < mapInfo.height + 1)
    {
        i = 1;
        while(i < mapInfo.width + 1)
        {
            vNeighbors = fGetNeighbors(i,j);
            //console.log(vNeighbors);
            if(map[j][i] === true){ // Alive
                if(aS[vNeighbors.alive] === true);
                else map[j][i] = false;
            }
            else{ // Dead
                if(aB[vNeighbors.alive] === true){ // B
                    map[j][i] = true;
                }
            }


            i++;
        }
        //console.log(temp);
        j++;
    }

    check.mapChange = true;

  }
  else{
    delay.mapChange = true;
  }
  }


  // (3)----------------update----------------------
  // 3-2-1
  {
  let hMap = document.getElementById("map");
  hMap.style.marginLeft = window.innerWidth - playerInfo.position.x;
  hMap.style.marginTop = window.innerHeight - playerInfo.position.y;

  hMap.style.transform = "rotate(" + playerInfo.direction + "deg)";
  hMap.style.transformOrigin = playerInfo.position.x + "px " +
                              playerInfo.position.y + "px";

  // mapDraw
  if(check.mapChange){
    let hDraw = hMap.getContext("2d");
    let bs = mapInfo.blockSize;

    //console.log(this.aMap);
    for(let j in map){
        //console.log("j = "+j);
        for(let i in map[0]){
            //console.log("i = "+i);
            if(map[j][i] === true){
              //console.log("alive draw");
              hDraw.fillStyle = "rgb(0,128,0)";
              hDraw.fillRect (i*bs, j*bs, bs, bs);
            }
            else{
              //console.log("dead draw");
              hDraw.fillStyle = "rgb(80,188,223)";
              hDraw.fillRect (i*bs, j*bs, bs, bs);
            }
        }
    }

    check.mapChange = false;
  }

  }


  //3-2-2;
  {


  }
  //--------------------------------------------
  startTime = Date().getTime();
}

/*
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
*/
