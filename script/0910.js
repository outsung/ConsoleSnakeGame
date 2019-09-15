// 1
{
  map = loadMap();
  player = userInfo();
}

// 2-1
{
  if(isKeyDown("a"))
  if(isKeyDown("w"))
  if(isKeyDown("s"))
  if(isKeyDown("d"))

  if(isKeyDown("i"))
  if(isKeyDown("VK_esc"))
}

// 2-2
{
  mousemove();
  click();
}

// 2-3
{
  resize();
}

// 2-4
{
  // 집
  {
    position : {}
  }
  // 나무
  {

  }

}

player(screenCenter)
{
  body(circle(image)){

  }
  head(img){

  }
  hand(attack?,weapon image){

  }
  bag(now inventory size, image){

  }
}

map(object)
{
  tree : {
    position : {
      x : 0,
      y : 0
    },
    stature : 0, // 0 ~ 3
    alpha : false // 반투명화
  },
  house : {
    position : {
      x : 0,
      y : 0
    },
    width : 0,
    height : 0,
    alpha : false
  },
  other : {
    position : {
      x : 0,
      y : 0
    },

  }
}
