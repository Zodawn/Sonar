import processing.serial.*;

Serial myPort;
String data="";
float angle, distance;

void setup() {
  size(800, 600);
  myPort = new Serial(this, "COM4", 9600);  // CHANGE COM PORT
  myPort.bufferUntil('.');
}

void draw() {
  fill(0);
  noStroke();
  rect(0, 0, width, height);

  translate(width/2, height*0.9);

  stroke(0,255,0);
  noFill();

  // Radar arcs
  arc(0,0,800,800,PI,TWO_PI);
  arc(0,0,600,600,PI,TWO_PI);
  arc(0,0,400,400,PI,TWO_PI);
  arc(0,0,200,200,PI,TWO_PI);

  // Radar line
  float rad = radians(angle);
  line(0,0,400*cos(rad),-400*sin(rad));

  // Object detection
  if(distance < 40){
    fill(255,0,0);
    float x = distance*10*cos(rad);
    float y = -distance*10*sin(rad);
    ellipse(x,y,10,10);
  }
}

void serialEvent(Serial myPort) {
  data = myPort.readStringUntil('.');
  if (data != null) {
    data = data.substring(0,data.length()-1);
    int commaIndex = data.indexOf(",");
    angle = float(data.substring(0,commaIndex));
    distance = float(data.substring(commaIndex+1));
  }
}
