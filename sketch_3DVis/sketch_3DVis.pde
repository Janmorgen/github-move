
// States:
// consts / numbers must be unique
final int HELP   = 0; // help 
final int GAME   = 1; // standard: selecting Cells with the mouse
int state = GAME;   // current 
 
// Cells 
CellClass[][][] Cells = new CellClass[32][16][16];
 
// rotation and Scale  
float currentAngle1;    
float currentAngle2;
float zoomScale1=1.7;         
boolean rotateFlag=false; 
 
// Misc 
PFont font1;

float middleX=0,middleY=0;
float sizeOfCubes=0;
float eyeX, eyeY, eyeZ;
float eyePX=0,eyePY=0,eyePZ=0;
float ang = 0;
int d = 200;
 
// --------------------------------------------------------
// main funcs 
 
void setup() {
  size(1400, 800, P3D);
  background(111);
 
  
  // define Cells
 
  float widthOverAll = 6*(height/10);
  float posX = (width/2) - (widthOverAll/2);
  println (posX);
 
  for (int i = 0; i < Cells.length; i++) {
    String[] lines = loadStrings(i+".dat");
    for (int j = 0; j < Cells[i].length; j++) {
      for (int k = 0; k < Cells[i][j].length; k++) {
        
      color currentCol = color (#0000FF);
        // prepare values 
        
      
      if(lines[k+j*Cells[i].length].equals("8")){
      currentCol = color (#FF0000);
      //System.out.println(lines[k+j*Cells[i].length]);

      }
      if(lines[k+j*Cells[i].length].equals("7")){
      currentCol = color (#33F9FF);
      //System.out.println(lines[k+j*Cells[i].length]);
      }
      if(lines[k+j*Cells[i].length].equals("6")){
      currentCol = color (#FBDD00);
      //System.out.println(lines[k+j*Cells[i].length]);
      }
      if(lines[k+j*Cells[i].length].equals("5")){
      currentCol = color (#ADADAD);
      //System.out.println(lines[k+j*Cells[i].length]);
      }
      if(lines[k+j*Cells[i].length].equals("4")){
      currentCol = color (#585858);
      //System.out.println(lines[k+j*Cells[i].length]);
      }
      if(lines[k+j*Cells[i].length].equals("3")){
      currentCol = color (#C6C6C6);
      //System.out.println(lines[k+j*Cells[i].length]);
      }
      if(lines[k+j*Cells[i].length].equals("2")){
      currentCol = color (#8A7133);
      //System.out.println(lines[k+j*Cells[i].length]);
      }
      if(lines[k+j*Cells[i].length].equals("1")){
      currentCol = color (#876200);
      //System.out.println(lines[k+j*Cells[i].length]);
      }
      if(lines[k+j*Cells[i].length].equals("0")){
      currentCol = color (#FFFFFF);
      //System.out.println(lines[k+j*Cells[i].length]);
      }


 
        // create a Cell        
        Cells[i][j][k] = new CellClass( 
          -185 + i*(height/10), 
          -230 + j*(height/10), 
          -230 + k*(height/10), 
          currentCol, 
          lines[k+j*Cells[i].length]);
      }
    }
  } // for
 
  font1 = createFont("Arial", 32);
  textFont(font1); 
  textAlign(CENTER, CENTER); 
 
  currentAngle1=0;
  currentAngle2=0;
} // func ---
 
void draw() {
  switch (state) {
 
  case HELP:
    // to do  
    background(111);
    text("Help\n\nThis is the help..."
      +"\n\npress any key.", 
      32, 344);
    break;
 
  case GAME:
    playTheGame();   
    break;
  } // switch
} // func draw()
 
// ----------------------------------------------------
 
void playTheGame() {
 
  background(111);
  lights();
 
  camera(0, 0, 510, 
    0,0,0, 
    0, 1, 0);
    translate(eyePX,eyePY,eyePZ);

 
  strokeWeight(1);
 
  //stroke(255, 0, 0); 
  //line (0, -100, 0, 
  //  0, height+100, 0); 
  //line (width/2, -100, 
  //  width/2, height+100 ); 
 
  // rotation  
  if (keyPressed&&key=='w')
    eyePZ+=10;
  if (keyPressed&&key=='s')
    eyePZ-=10;
  if (keyPressed&&key==' ') {
    eyePY+=10;
  }
  if (keyPressed&&keyCode==SHIFT) {
    eyePY-=10;
  }
  if (keyPressed&&key=='a') {
    eyePX+=10;
  }
  if (keyPressed&&key=='d') {
    eyePX-=10;
  }
   if (keyPressed&&key=='o') {
    translate(8, 16);
    currentAngle1+=2;

  }  
  if (keyPressed&&key=='p') {
    translate(8, 16);
    currentAngle1-=2;

  }
   if (keyPressed&&key=='[') {
    translate(8, 16);
    currentAngle2+=2;

  }  
  if (keyPressed&&key==']') {
    translate(8, 16);
    currentAngle2-=2;

  }
   if (keyPressed&&key==',') {
     if((sizeOfCubes-1)==-79){
       sizeOfCubes=79;
       sizeOfCubes=sizeOfCubes%80;
     }else{
      sizeOfCubes=(sizeOfCubes-1)%80;
     }

  }
   if (keyPressed&&key=='.') {
     if((sizeOfCubes+1)==79){
       sizeOfCubes=-79;
       sizeOfCubes=sizeOfCubes%80;
     }else{
      sizeOfCubes=(sizeOfCubes+1)%80;
     }
  }
  
  
    pushMatrix(); 
  scale (zoomScale1);
   rotateY(radians(currentAngle1));

  showTheGrid();
 
  popMatrix();



  //println("Angle "+ang+": "+eyeX+" / "+eyeY+" / "+eyeZ);

    
 
  if (rotateFlag)
    currentAngle1++;
 
  // 2D part / HUD  ---------------------------------------------
  camera();
  //  hint(DISABLE_DEPTH_MASK);
  //  hint(DISABLE_DEPTH_TEST);
  noLights();
  // textMode(MODEL);
  // textMode(SHAPE);
  textSize(14);
  fill(0); 

 
  //stroke(255, 0, 0); 
  //line (width/2, -100, 
  //  width/2, height+100 );
}
 
// ------------------------------------------------------------------
 
void showTheGrid() {
  // show Cells : all
  for (int i = 0; i < Cells.length; i++) {
    for (int j = 0; j < Cells[i].length; j++) {
      for (int k = 0; k < Cells[i][j].length; k++) {
        //
        Cells[i][ j][k].show();
        //
      }// for
    }// for
  }// for
} // func 
 
// ----------------------------------------------------
// input funcs
 
void keyPressed () {
 
  switch (state) {
 
  case HELP : 
    // reset 
    state = GAME; 
    break; 
 
  case GAME : 
    if (key!=CODED) {
 
      // not CODED -----------------------------------
 
      if (key=='x'||key=='X') {
        // reset  
        //defineCells();
      } else if (key == 'h' ) {
        //
        // F1 
        state = HELP; 
        println("1");
      } else if (key == 'r' ) {
        //
        // F1 
        rotateFlag = !rotateFlag; 
        println("1");
      }
    } else {
 
      // if (key==CODED) { --------------------------------------------
      //
      switch (keyCode) {
 
      case java.awt.event.KeyEvent.VK_PAGE_UP : 
        zoomScale1-=.01; 
        break; 
 
      case java.awt.event.KeyEvent.VK_PAGE_DOWN : 
        zoomScale1+=.01; 
        break; 
 
      default : 
        // do nothing 
        break;
      } // switch
    } // else
    break;
 
    //
  } // switch (state) { 
  //
} // func 
 
// ----------------------------------------------------
// misc funcs
 
//void defineCells() {
//  // define Cells
 
//  float widthOverAll = 6*(height/10);
//  float posX = (width/2) - (widthOverAll/2);
//  println (posX);
 
//  for (int i = 0; i < Cells.length; i++) {
//    for (int j = 0; j < Cells[i].length; j++) {
//      for (int k = 0; k < Cells[i][j].length; k++) {
 
//        // prepare values 
//        color currentCol = color (111);
//        String randomWord="";
//        randomWord+=""+char(int(random(65, 95)));
//        randomWord+=""+char(int(random(65, 95)));
//        randomWord+=""+char(int(random(65, 95)));
//        randomWord+=""+char(int(random(65, 95)));
 
//        // create a Cell        
//        Cells[i][ j][k] = new CellClass( 
//          -230 + i*(height/10), 
//          -185 + j*(height/10), 
//          -230 + k*(height/10), 
//          currentCol, 
//          randomWord);
//      }
//    }
//  } // for
//} // func 
 
// ----------------------------------------
 
void mousePressed() {
 
  switch (state) {
 
  case HELP :
    // go back 
    state = GAME; 
    break; 
 
  case GAME : 
    //
    break;
  } // switch
}//func
 
void mouseWheel(MouseEvent event) {
 
  //This method controls zoomScale1 variable
 
  if (event.getCount()==-1)
    zoomScale1 += 0.1;
  else if (event.getCount()==1) {
    if (zoomScale1>=.1)
      zoomScale1 -= 0.1;
  }
 
  // check lower boundary 
  if (zoomScale1<.1)
    zoomScale1 = .1;
} 
 
// ========================================================
// classes
 
class CellClass {
 
  // this class represents one Box / Cell
 
  float x; 
  float y; 
  float z; 
 
  float scX, scY; 
 
  color col; 
  color defaultCol; 
 
  String textCell=""; 
 
  // constr
  CellClass(float y_,float z_, float x_, 
    color col_, 
    String word_) {
    x = x_; 
    y = y_; 
    z = z_; 
    col = col_;
    defaultCol = col; 
 
    textCell=word_;
  } // constr
 
  void show() {
    pushMatrix(); 
 
    translate(x, y, z);
    rotateY (radians(  currentAngle2) );
 
    //noFill();
    fill(defaultCol);//blue 
    noStroke();
    //stroke(defaultCol); 
    int size=(int)sizeOfCubes;
    box(size,size,size);
 
    
 
    //noStroke(); 
    // Cell(7);
 
    //text(textCell, 0, 0); 
    col = defaultCol;
 
    // the Cell / box was drawn at (0, 0, 0), store that location
    scX = screenX(0, 0, 0); 
    scY = screenY(0, 0, 0);
 
    popMatrix();
  } // method
} // class 
// ============================
