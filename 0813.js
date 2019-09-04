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

// 내장 함수
//사이즈 변경하면 실행하는 함수
function resize() {
  if(unNameVariable === true){
    let hUserScreen = document.getElementById("userScreen");
    console.log('Resizing...')

    userScreen.width = window.innerWidth;
    userScreen.height = window.innerHeight;

    hUserScreen.style.width = userScreen.width + "px";
    hUserScreen.style.height = userScreen.height + "px";



    let hPlayer = document.getElementById("player");
    hPlayer.style.marginLeft = userScreen.width / 2 - 5 + "px";
    hPlayer.style.marginTop = userScreen.height / 2 - 5 + "px";
  }
};

//키 입력 시 실행하는 함수
function keyPress(){
  if(unNameVariable === true){
    let downKeyCode = event.keyCode;
    //console.log(downKeyCode);

    if (downKeyCode in keyCode){
      keyCode[downKeyCode] = 1;
    }
    else{
      event.returnValue = false; // 브라우저 기능 키 무효화
    }
    //console.log(keyCode);
  }
}

function keyUp(){
  if(unNameVariable === true){
    let upKeyCode = event.keyCode;
    //console.log(upKeyCode);

    if (upKeyCode in keyCode){
      keyCode[upKeyCode] = 0;
    }
    //console.log(keyCode);
  }
}

function getRandomInt(min, max){
  return Math.floor(Math.random() * (max - min)) + min;
}

function getRandomBool(threshold){
    return threshold >= getRandomInt(1,101);
}

function fplay(){
  this.vStart
}


function fMap(width, height, initV){
    this.width = width;
    this.height = height;
    this.initV = initV;

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
                    temp.push(false);
                    i++;
                }
                else{
                    temp.push(getRandomBool(this.initV));
                    i++;
                }
            }
            console.log(temp);
            res.push(temp);
            j++;
        }
        console.log("x "+this.width+" y "+this.height+" setting..");

        return res;
    };


    this.aMap = this.getMapA();

    // 세포자동화
    this.fMapSellularAutomata = function(rule){

        let B = [false, false, false, false, false, false, false, false, false, false];
        let S = [false, false, false, false, false, false, false, false, false, false];

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
                    if(j === y || i === x){
                        i++;
                        continue;
                    }
                    else{
                        if(this.aMap[j][i]) vNeighbors.alive++;
                        else vNeighbors.dead++;
                    }
                    i++;
                }
                j++;
            }

            if(vNeighbors.dead + vNeighbors.alive !== 8) console.log("getNei.. error roop!!");
            else return vNeighbors;
        }


        let ruleB = rule.split("/").[0].split;
        let ruleS = rule.split("/").[0].split;

        for(let i in ruleB)
        {
            if(typeof(i =* 1) === "number") B[i] = true;
            else{
                if(i !== "B"){
                    console.log("fMapSellularAutomata-[B]error-!");
                    return -1;
                }
            }
        }
        for(let i in ruleS)
        {
            if(typeof(i =* 1) === "number") S[i] = true;
            else{
                if(i !== "S"){
                    console.log("fMapSellularAutomata-[S]error-!");
                    return -1;
                }
            }
        }


        //start
        let j = 1;
        let i = 1;
        let vNeighbors = {};

        while(j < this.height + 1)
        {
            i = 1;
            while(i < this.width + 1)
            {
                vNeighbors = fGetNeighbors(i,j);

                if(this.aMap[j][i] === true){ // Alive
                    if(S[vNeighbors.alive] === true);
                    else this.aMap[j][i] = false;
                }
                else{ // Dead
                    if(B[vNeighbors.alive] === true){ // B
                        this.aMap[j][i] = true;
                    }
                }


                i++;
            }
            console.log(temp);
            j++;
        }

    }


    this.fDrawMap = function(){

    }
}


let vMap = new fMap(10,10);
console.log(vMap.aMap[5][5]);

//vMap.fMapSellularAutomata("B23/S45");
