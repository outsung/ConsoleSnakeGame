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


function getRandomInt(min, max){
  return Math.floor(Math.random() * (max - min)) + min;
}

function getRandomBool(threshold){
    return threshold >= getRandomInt(1,101);
}


function fMap(width, height){
    this.width = width;
    this.height = height;
        
    this.getMapA = function(){
        let j = 0;
        let i = 0;
        let res = new Array;
        let temp = new Array;
    
        while(j < this.width)
        {
            i = 0;
            temp = new Array;
            while(i < this.height)
            {
                temp.push(getRandomBool(65));
                i++;
            }
            console.log(temp);
            res.push(temp);
            j++;
        }
        console.log("x "+this.width+" y "+this.height+" setting..");
        
        return res;
    };
    
    this.aMap = this.getMapA();
    
    this.fMapSellularAutomata = function(){
        
        let j = 0;
        let i = 0;
        
        while(j < this.width)
        {
            i = 0;
            while(i < this.height)
            {
                if(getRandomBool){
                    
                }
                i++;
            }
            console.log(temp);
            j++;
        }
        
    }
    
    
}


let vMap = new fMap(10,10);
console.log(vMap.aMap[5][5]);