/*
let square = {
    leftTop : {
        x : 0,
        y : 0
    },
    rightBottom : {
        x : 0,
        y : 0
    },
}
let object = {
    position : 0,
    wall : [{
    }]
}
*/

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
    let downKeyCode = event.keyCode;
    console.log(downKeyCode);

    if (downKeyCode in oMain.oPlay.keyCode){
      oMain.oPlay.keyCode[downKeyCode] = true;
    }
    else{
      event.returnValue = false; // 브라우저 기능 키 무효화
    }
    //console.log(keyCode);
  }
}

function keyUp(){
  if(oMain.oPlay.vStart === true){
    let upKeyCode = event.keyCode;
    //console.log(upKeyCode);

    if (upKeyCode in oMain.oPlay.keyCode){
      oMain.oPlay.keyCode[upKeyCode] = false;
    }
    //console.log(keyCode);
  }
}


function isKeyDown(key){
  if(oMain.oPlay.vStart === true) return oMain.oPlay.keyCode[key];
  else console.log("iskeydown error!!");
}

//----------------------------------------


function getRandomInt(min, max){
  return Math.floor(Math.random() * (max - min)) + min;
}

function getRandomBool(threshold){
    return threshold >= getRandomInt(1,101);
}




//--------------상황------------

function fGame(){ // 게임 시작

    // 마우스 위치?
    this.vStart = true;
}


function fPlay(){ // 게임 플레이

    // 마우스 고정 & 마우스
    this.vStart = true;

    //https://blog.outsider.ne.kr/322
    this.keyCode = {
    27 : false,
    87 : false,
    65 : false,
    83 : false,
    68 : false,
    122 : false,
    123 : false
    };

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
    this.getMapA = function(){
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


    this.aMap = this.getMapA();

    // 세포자동화
    this.fMapSellularAutomata = function(rule){

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


    this.fDrawMap = function(){

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
//-------------------------------




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
