#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    int gameState;
    
    // clickTosStart相关
    int width, height;
    ofImage pressToStart;
    ofTrueTypeFont pressToStartString, copyRight;
    
    // ui
    ofTrueTypeFont healthString, awakeString, timeString, scoreString;
    //int startTime;
    int minute, second, hour, psecond;
    float score, health, awake;
    ofImage face;
    
    // game
    ofImage background, gameBackground;
    ofImage hand, handHit;
    ofFbo hitMask;
    bool bHit;
    // 控制帧动画时间
    float pTime, nowTime;
    float pHealthTime, nowHealthTime;
    ofTrueTypeFont gameOverString;
};
