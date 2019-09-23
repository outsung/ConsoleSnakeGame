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

let rect = {
  left : 0,
  top : 0,
  right : 0,
  bottom : 0
}

let circle = {
  x : 0,
  y : 0,
  r : 0
}

function isIntersectCP(c1,p1){
  let deltax = c1.x - p1.x;
  let deltay = c1.y - p1.y;

  if( (deltax * deltax + deltay * deltay) > c1.r ) return false;
  return true;
}

function isIntersectCR(c1,r1){
  if( ("x" in c1) && ("y" in c1) && ("r" in c1) &&
    ("left" in r1) && ("top" in r1) && ("right" in r1) && ("bottom" in r1) ){

    if( (r1.left <= c1.x && c1.x <= r1.right) ||
      (r1.top <= c1.y && c1.y <= r1.bottom) ){

      r1.left - c1.r;
      r1.top - c1.r;
      r1.right - c1.r;
      r1.bottom - c1.r;

      if( (r1.left < c1.x && c1.x < r1.right) &&
        (r1.top < c1.y && c1.y < r1.bottom) ){
          return true;
      }

    }
    else{
      let tempP = {
        x : 0,
        y : 0
      };
      tempP.x = r1.left;
      tempP.y = r1.top;
      if(isIntersectCP(c1,tempP)) return true;

      tempP.x = r1.left;
      tempP.y = r1.bottom;
      if(isIntersectCP(c1,tempP)) return true;

      tempP.x = r1.right;
      tempP.y = r1.top;
      if(isIntersectCP(c1,tempP)) return true;

      tempP.x = r1.right;
      tempP.y = r1.bottom;
      if(isIntersectCP(c1,tempP)) return true;

    }

    return false;

  }
  else {
    console.log("--isIntersectCR_error!--");
    return -1
  }
}

function isIntersectCC(c1,c2){

}

/*
function aa(){
  let i = 0;
  let j = 0;
  while(j > ){
    while () {
        map[][]
    }
  }
}
*/

function getDistancePP(p1,p2){
  if( ("x" in p1) && ("y" in p1) &&
    ("x" in p2) && ("y" in p2) ){
    let deltax = p1.x - p2.x;
    let deltay = p1.y - p2.y;

    return Math.sqrt(deltax * deltax + deltay * deltay);
  }
  else{
    console.log("--getDistancePP_error!--");
    return -1
  }
}

function kMeansClustering(k, x){
  let min;
  let total;

  let i = 0;
  let j = 0;
  let t = 0;

  let nc = [];
  let c = [];
  let r = []; //Xc

  i = 0;
  while(i < k){
    c[i] = {
      x : getRandomInt(0, (mapInfo.width + 2) * mapInfo.blockSize),
      y : getRandomInt(0, (mapInfo.height + 2) * mapInfo.blockSize),
      count : 0
    };
    i++;
  }
  //console.log(c);
  t = 0;
  while(t < 100){
    j = 0;
    i = 0;
    while(j < x.length){
      min = -1;
      i = 0;
      while(i < c.length){
        if( (min === -1) || (min > getDistancePP(c[i], x[j])) ){
          min = getDistancePP(c[i], x[j]);
          r[j] = i;
        }
        i++;
      }
      j++;
    }

    nc = c;

    j = 0;
    while(j < r.length){
      nc[r[j]].x += x[j].x;
      nc[r[j]].y += x[j].y;
      nc[r[j]].count += 1;
      j++;
    }

    i = 0;
    while(i < c.length){
      nc[i].x = nc[i].x / nc[i].count;
      nc[i].y = nc[i].y / nc[i].count;
      i++;
    }

    //최적화!
    i = 0;
    while(i < c.length){
      nc[i].x = Math.floor(nc[i].x * 10000) / 10000;
      nc[i].y = Math.floor(nc[i].y * 10000) / 10000;
      i++
    }

    i = 0;
    while(i < c.length){
      if(nc[i].x !== c[i].x || nc[i].y !== c[i].y){
        break;
      }
      i++
    }
    if(i == c.length) return [r, c];


    t++;
  }

  return [r, c];
}

function getMapAlive(){
  let j = 0;
  let i = 0;

  let res = [];

  j = 0;
  while(j < mapInfo.height){
    i = 0;
    while(i < mapInfo.width){
      if(map[j][i] != 0){
        res.push({
          x : (i * mapInfo.blockSize) + (mapInfo.blockSize / 2),
          y : (j * mapInfo.blockSize) + (mapInfo.blockSize / 2)
        });
      }
      i++;
    }
    j++;
  }

  return res;
}


//------------------------------------------
let gameplay = true;
let check = {
  inventory : false,
  setting : false,
  mapChange : true,
  kMeans : false
}

let keyCode = {
  27 : false, //esc

  87 : false, //w
  65 : false, //a
  83 : false, //s
  68 : false, //d

  78 : false, //n
  77 : false, //m
  73 : false, //i
  72 : false, //h

  49 : false, //1
  50 : false //2

  //122 : false,
  //123 : false
}

let playerInfo = {
  position : {
    x : 0,
    y : 0
  },
  direction : 0,
  speed : 5,
  mouseSensitivity : 10
}

//나중에 scale 변경 할 때 같이 변경됨

let map = new Array;

//"B678/S4567" -> road (58) ?
//B5678/S45678 -> cave 수정
//B4678 /S35678 -> forest, init(29, 25~30), Cycle(1),
//B5678/S24567 -> city, init(60, 100) ->
//B2478/S0145678 -> house init(20)
let mapInfo = {
  rule : "B5678/S45678",
  width : 200,
  height : 200,
  blockSize : 10,
  initV : 40,
  placeCount : 6,
  place : new Array
}

let blockInfo = {
  dead : "rgb(80,188,223)",
  "c1" : "rgb(100,128,0)",
  "c2" : "rgb(63,11,27)",
  "c3" : "rgb(122,22,49)",
  "c4" : "rgb(207,66,60)",
  "c5" : "rgb(252,125,73)",
  "c6" : "rgb(255,212,98)"
}

// true = 가능
// false = 불가능
let delay = {
  keyI : true,
  keyEsc : true,

  key1 : true,
  key2 : true,

  keyN : true,
  keyM : true,
  keyH : true,

  update : 10,
  attack : true
}

let object = {
  house : rect,
  tree : circle
}


//--thread-------------------------------

//키 관련 함수 keyCode[] 중심
function keyPress(){
  if(gameplay === true){
    let down = event.keyCode;
    //console.log(down);

    if (down in keyCode){
      console.log(down+"down");
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
      console.log(up+"up");
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
      return keyCode[key];
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
    hPlayer.style.marginLeft = (window.innerWidth / 2) - 5 + "px";
    hPlayer.style.marginTop = (window.innerHeight / 2) - 5 + "px";
  }
};


//define
//(2-2)
{
let hUserScreen = document.getElementById("userScreen");

//(2-2-1)
hUserScreen.addEventListener('click', function(event){
  if(check.inventory || check.setting) {

  }
  else {
    hUserScreen.requestPointerLock();
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
            temp.push(0);
            i++;
        }
        else{
            if(getRandomBool(mapInfo.initV)) temp.push(1);
            else temp.push(0);
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

resize();
}

function mainLoop(){

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

//이동 했을 때 position update
if((normalizeX != 0) || (normalizeY != 0)){
  ridian = Math.radians(playerInfo.direction - directionDelta);

  // parseFloat (string -> float)
  // toFixed (x) x자리만 표시
  delta.x = parseFloat(Math.cos(ridian).toFixed(1));
  delta.y = parseFloat(Math.sin(ridian).toFixed(1));


  // 충돌 감지
  if(true){

    //이동
    playerInfo.position.x += (delta.x * playerInfo.speed);
    playerInfo.position.y -= (delta.y * playerInfo.speed);
  }
  else {
    // sliding vector 이용

  }
}
}

// 2-1-1, 2-1-2
{

if(isKeyDown("i") && delay.keyI){
  delay.keyI = false;
  check.inventory = !check.inventory;
  console.log(check.inventory+"key");
  // key code reset
}
else if(!isKeyDown("i") && !delay.keyI){
  delay.keyI = true;
}

// esc
if(isKeyDown(27) && delay.keyEsc){
  delay.keyEsc = false;
  check.setting = !check.setting;
  // key code reset
}
else if(!isKeyDown(27) && !delay.keyEsc){
  delay.keyEsc = true;
}

if(isKeyDown("m") && delay.keyM){
  delay.keyM = false;
  //mapInfo.rule
  //CellularAutomata
  {
  let aB = [false, false, false, false, false, false, false, false, false];
  let aS = [false, false, false, false, false, false, false, false, false];

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
                  if(map[j][i] != 0) vNeighbors.alive++;
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


  let sRuleB = mapInfo.rule.split("/")[0].split("");
  let sRuleS = mapInfo.rule.split("/")[1].split("");

  //console.log(sRuleB);
  for(let i in sRuleB)
  {
      //console.log(sRuleB[i]);
      if(!isNaN(sRuleB[i] *= 1)) aB[sRuleB[i]] = true;
      else{
          //console.log("a");
          //if(sRuleB[i] !== "B"){
          //    console.log("fMapSellularAutomata-[B]error-!");
          //}
      }
  }
  //console.log(sRuleS);
  for(let i in sRuleS)
  {
      if(!isNaN(sRuleS[i] *= 1)) aS[sRuleS[i]] = true;
      else{
          if(sRuleS[i] === "S" ){
            console.log("error");
          }
          //if(sRuleS[i] !== "S"){
          //    console.log("fMapSellularAutomata-[S]error-!");
          //}
      }
  }

  console.log(aB, aS);
  //start
  let j = 1;
  let i = 1;
  let vNeighbors = {};
  let tempMap = map;

  while(j < mapInfo.height + 1)
  {
      i = 1;
      while(i < mapInfo.width + 1)
      {
          vNeighbors = fGetNeighbors(i,j);
          //console.log(vNeighbors);
          if(map[j][i] != 0){ // Alive
              if(aS[vNeighbors.alive] === true)
                tempMap[j][i] = 1;
              else tempMap[j][i] = 0;
          }
          else{ // Dead
              if(aB[vNeighbors.alive] === true){ // B
                  tempMap[j][i] = 1;
              }
              else tempMap[j][i] = 0;
          }


          i++;
      }
      //console.log(temp);
      j++;
  }
  map = tempMap;
  }
  check.mapChange = true;

}
else if(!isKeyDown("m") && !delay.keyM){
  delay.keyM = true;
}


if(isKeyDown("h") && delay.keyH){

  delay.keyH = false;
}
else if(!isKeyDown("h") && !delay.keyH){
  delay.keyH = true;
}


if(isKeyDown("n") && delay.keyN){
  delay.keyN = false;
  let t = 0;

  let x = getMapAlive();
  //console.log(x);
  let kMeans = kMeansClustering(mapInfo.placeCount, x);
  let r = kMeans[0];

  mapInfo.place = kMeans[1];

  console.log(r);


  t = 0;
  while(t < r.length){
    map[(x[t].y / mapInfo.blockSize - 0.5)]
      [(x[t].x / mapInfo.blockSize - 0.5)] = r[t] + 1;
    t++;
  }

  check.kMeans = true;
  check.mapChange = true;
}
else if(!isKeyDown("n") && !delay.keyN){
  delay.keyN = true;
}


}



// (3)----------------update----------------------

// 3-2-1
{
let hMap = document.getElementById("map");
hMap.style.marginLeft = (window.innerWidth / 2) - playerInfo.position.x + "px";
hMap.style.marginTop = (window.innerHeight / 2) - playerInfo.position.y + "px";

hMap.style.transform = "rotate(" + playerInfo.direction + "deg)";
hMap.style.transformOrigin = playerInfo.position.x + "px " +
                            playerInfo.position.y + "px";

// mapDraw
if(check.mapChange){
  console.log("Map change");
  let hDraw = hMap.getContext("2d");
  let bs = mapInfo.blockSize;

  //console.log(this.aMap);
  for(let j in map){
      //console.log("j = "+j);
      for(let i in map[0]){
          //console.log("x : " + i + " y : " + j + " c : " + map[j][i]);
          if(map[j][i] != 0){
            //console.log("alive draw");
            if("c" + map[j][i] in blockInfo){
              hDraw.fillStyle = blockInfo["c" + map[j][i]];
            }
            else hDraw.fillStyle = blockInfo["c0"];
            hDraw.fillRect (i*bs, j*bs, bs, bs);
          }
          else{
            //console.log("dead draw");
            hDraw.fillStyle = blockInfo.dead;
            hDraw.fillRect (i*bs, j*bs, bs, bs);
          }
      }
  }

  check.mapChange = false;
}
if(check.kMeans){
  let hDraw = hMap.getContext("2d");
  let bs = mapInfo.blockSize;

   circle.arc(100, 35, 25, 0, 2 * Math.PI);

  check.kMeans = false;
}

}


//3-2-2;
{


}


}
//--------------------------------------------
let hInputKey = setInterval(mainLoop, delay.update);

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
