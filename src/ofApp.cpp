#include "ofApp.h"

enum e_gameState
{
    GAME_CLICK_TO_START = 0,
    GAME_STARING,
    GAME_RESTART,
    GAME_STOP,
    
    GAME_STATE_COUNT
};

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowShape(2048, 1536);
    
    gameState = GAME_CLICK_TO_START;
    
    pressToStart.loadImage("clickToStart.png");
    face.loadImage("3.jpg");
    hand.loadImage("hand.png");
    handHit.loadImage("handHit.png");
    background.loadImage("background.jpg");
    gameBackground.loadImage("gameBackground.png");
    
    hitMask.allocate(1, 1);
    hitMask.begin();
    ofClear(220, 40, 40, 180);
    hitMask.end();
    
    pressToStartString.loadFont("3.ttf", 50);
    copyRight.loadFont("3.ttf", 20);
    healthString.loadFont("3.ttf", 45);
    awakeString.loadFont("3.ttf", 45);
    scoreString.loadFont("3.ttf", 45);
    timeString.loadFont("3.ttf", 45);
    gameOverString.loadFont("3.ttf", 60);
    
    pTime = nowTime = pHealthTime = nowHealthTime = 0;
    minute = second = hour = psecond = 0;
    awake = 100;
    score = 0;
    health = 100;
}

//--------------------------------------------------------------
void ofApp::update(){
    width = ofGetWidth();
    height = ofGetHeight();
    
    switch (gameState)
    {
        case GAME_CLICK_TO_START:
        case GAME_STOP:
        case GAME_RESTART:
            break;
            
        case GAME_STARING:
            if(health <= 0)
                gameState = GAME_STOP;
            // 健康更新
            // 一段时间后再更新状态
            static bool bFirst = true;
            if(bFirst)
            {
                pHealthTime = ofGetElapsedTimef();
                nowHealthTime = ofGetElapsedTimef();
                
                pTime = ofGetElapsedTimef();
                nowTime = ofGetElapsedTimef();
                
                bFirst = false;
            }
            
            // 健康自动回复
            nowHealthTime = ofGetElapsedTimef();
            if(nowHealthTime - pHealthTime > 1.0)
            {
                health ++;
                
                pHealthTime = nowHealthTime;
            }
            
            // 拳击时间更新也在这里
            nowTime = ofGetElapsedTimef();
            if(nowTime - pTime >= 1.0)
            {
                if(bHit)
                {
                    health -= 10;
                    awake += 5;
                    bHit = false;
                }
                
                pTime = nowTime;
            }
            
            if(health >= 100)
                health = 100;
            if(awake >= 100)
                awake = 100;
            
            // 时间更新
            if(second == 0)
                psecond = 0;
            
            if(second - psecond >= 1.0)
            {
                psecond = second;
                awake -= 2;
                if(awake <= 0)
                {
                    gameState = GAME_STOP;
                }
            }
            break;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    switch (gameState) {
        case GAME_CLICK_TO_START:
            ofBackground(ofColor(255));
            
            //background.draw(0, 0, width, height);
            pressToStart.draw(width/2 - pressToStart.getWidth()/2, height/2 - pressToStart.getHeight()/2);
            
            ofPushStyle();
            ofSetColor(200, 20, 20);
            pressToStartString.drawString("Press To Start", width*3.0f/7.0f, height/2.0f);
            ofSetColor(20, 20, 20);
            copyRight.drawString("All rights not reserved.Buguan you see understand or see don't understand.\nI dou would write zhexie hua down.", width/3.0f, height*2.0f/3.0f);
            ofPopStyle();
            break;
            
        case GAME_STARING:
        {
            ofBackground(ofColor(255));
            //background.draw(0, 0, width, height);
            gameBackground.draw(0, 0, width, height);
            
            // game
            // 此处用帧动画代替
            if(!bHit)
            {
                hand.draw(0.0f, height*5.0f/6.0f - hand.getHeight());
            }
            else
            {
                hitMask.draw(0, 0, width, height);
                handHit.draw(width/2.0f - handHit.getWidth()/2.0f, height/2.0f - handHit.getHeight()/2.0f);
            }
            
            // ui
            ofPushStyle();
            
            ofSetColor(80, 80, 190);
            ofRect(0, height*5.0f/6.0f, width, height/6.0f);
            
            float uiHeight = height*5.0/6.0f;
            float fontHeight = height*5.0/6.0f + height/18.0f, intHeight = height*5.0/6.0f + height*3.0f/24.0f;
            
            ofSetColor(200);
            // health
            // x: 1/8 widget width y: 1/3 widget height
            healthString.drawString("Health", width/72.0f, fontHeight);
            // x: 1/8 widget width y: 3/4 widget height
            healthString.drawString(ofToString(health), width/72.0f, intHeight);
            // x: 1/8 widget width y: 1/1 widget height
            ofLine(width/9.0f, uiHeight, width/9.0f, height);
            
            // awake
            awakeString.drawString("Awake", width/9.0f+width/72.0f, fontHeight);
            // x: 1/8 widget width y: 1/3 widget height
            //awakeString.drawString(ofToString(awake), width/9.0f+width/72.0f, intHeight);
            // 进度条
            ofPushStyle();
            
            ofSetColor(200);
            ofRect(width/9.0f+width/72.0f, intHeight-width/72.0f, width*3.0f/9.0f-width/36.0f, 20);
            ofSetColor(40, 190, 40);
            ofRect(width/9.0f+width/72.0f, intHeight-width/72.0f, (width*3.0f/9.0f-width/36.0f)*awake/100.0f, 20);
            //printf("%d\n", awake);
            
            ofPopStyle();
            
            // face
            face.draw(width*4.0/9.0f, uiHeight, width/9.0f, height/6.0f);
            ofLine(width*4.0/9.0f, uiHeight, width*4.0f/9.0f, height);
            
            // time
            static float startTime = ofGetElapsedTimef();
            
            // 时间傻逼更新
            second = ofGetElapsedTimef() - startTime - minute * 60;
            if(second >= 60)
            {
                second = 0;
                minute++;
            }
            
            timeString.drawString("Time", width*5.0f/9.0f+width/72.0f, fontHeight);
            // 根据分 秒手工算出
            if(second < 10)
                timeString.drawString("00:0" + ofToString(minute) + ":0" + ofToString(second), width*5.0f/9.0f+width/72.0f, intHeight);
            else
                timeString.drawString("00:0" + ofToString(minute) + ":" + ofToString(second), width*5.0f/9.0f+width/72.0f, intHeight);
            
            ofLine(width*5.0/9.0f, uiHeight, width*5.0f/9.0f, height);
            
            //score
            scoreString.drawString("Score", width*7.0/9.0f+width/72.0f, fontHeight);
            scoreString.drawString(ofToString(score), width*7.0/9.0f+width/72.0f, intHeight);
            ofLine(width*7.0/9.0f, uiHeight, width*7.0f/9.0f, height);
            
            ofPopStyle();
            break;
        }
        case GAME_STOP:
            ofPushStyle();
            background.draw(0, 0, width, height);
            
            ofSetColor(ofColor(220));
            gameOverString.drawString("You Lose  You Died  You Bad Bad\nDon't Try Again", width/64.0f, height/2.0);
            ofPopStyle();
            
            printf("你死了……");
            break;
            
        case GAME_RESTART:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key == OF_KEY_RETURN)
    {
        gameState = (gameState + 1) % GAME_STATE_COUNT;
        printf("Game Start!");
    }
    else if(key == ' ')
    {
        if(gameState == GAME_STARING)
            bHit = true;
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
