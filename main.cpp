
#include <iostream>
#include <random>
#include <vector>
#include <cmath>
#include <ncurses.h>
#include <fstream>
#define WIDTH 16
#define HEIGHT 16
#define DEPTH 32
#define WORLD_SEED 2394237

#define NUMBEROFBLOCKS 9


enum blocks{
        Empty,
        Wood,
        Dirt,
        Stone,
        Coal,
        Iron,
        Gold,
        Diamond,
        Player
};

struct PlayerObj{
    int xPos=0;
    int yPos=0;
    int zPos=0;
    blocks standingBlock=Empty;

};



std::discrete_distribution<int> setBlockDistribution(int blockDepth,blocks frontBlock=Empty,blocks leftBlock=Empty,blocks topBlock=Empty){
    //    std::cout<<"Got to getBlock";
    std::vector<double> blockProbabilities;
    std::vector<double>::iterator probabilityIndex;
    probabilityIndex =blockProbabilities.begin();
    // Values from .4 to 10 acceptable
    // .4 roughly represents 100%
    double blockDepthWeight=1;
    double lastBlockWeight=2;

    // Default value is: Empty(1)
    // States the common block which should have the highest probability at this depth
    blocks levelBlock=Empty;

    if(0<((blockDepth+1)/DEPTH)&&((blockDepth+1)/DEPTH)<=.2){}
    else if (0.2<((blockDepth+1)/(double)DEPTH)&&((blockDepth+1)/(double)DEPTH)<=.3){
        levelBlock=Dirt;
    }
    else if (0.3<((blockDepth+1)/(double)DEPTH)&&((blockDepth+1)/(double)DEPTH)<=.8){
        levelBlock=Stone;
    }
    else if (0.8<((blockDepth+1)/(double)DEPTH)&&((blockDepth+1)/(double)DEPTH)<=1){
        levelBlock=Iron; 
    }
    

    for(int i=0;i<NUMBEROFBLOCKS-1;i++){
    // Caclulated from the intersection of two different normal distributions
    // One accounts for lastBlock and the other accounts for levelBlock
    //        double blockProbability=1;
        double blockProbability= ((1/(blockDepthWeight*sqrt(2*3.14159)))*pow(2.71828,-pow(i-levelBlock,2)/(2*pow(blockDepthWeight,2)))
                                 +(1/(lastBlockWeight*sqrt(2*3.14159)))*pow(2.71828,-pow(i-leftBlock,2)/(2*pow(lastBlockWeight,2)))
                                 +(1/(lastBlockWeight*sqrt(2*3.14159)))*pow(2.71828,-pow(i-frontBlock,2)/(2*pow(lastBlockWeight,2)))
                                 +(1/(lastBlockWeight*sqrt(2*3.14159)))*pow(2.71828,-pow(i-topBlock,2)/(2*pow(lastBlockWeight,2))))*100;
    //        std::cout<<blockProbability<<std::endl;
        blockProbabilities.push_back(blockProbability);
    }
        std::discrete_distribution<int> distribution(blockProbabilities.begin(),blockProbabilities.end());
    

    //    std::cout<<distribution(generator);
    return distribution;

    


}
void generateWorld(int array[DEPTH][HEIGHT][WIDTH]){
    std::default_random_engine generator(WORLD_SEED);
    //std::cout<<"Got to generateWorld";
    for(int z=0;z<DEPTH;z++){
        for(int y=0;y<HEIGHT;y++){
            for(int x=0;x<WIDTH;x++){
                if(z>1){
                    // This if statement makes sure that blocks with a previous block always count on it
                    //  Make this so it is more about adjacentcy rather than dead on programmic last block
                    if(y!=0){
                        if(x==0){
                            array[z][y][x]=setBlockDistribution(z, (blocks)array[z][y-1][x],(blocks)array[z][y-1][WIDTH-1],(blocks)array[z-1][y][x])(generator);
                        }else{
                            array[z][y][x]=setBlockDistribution(z, (blocks)array[z][y-1][x],(blocks)array[z][y][x-1],(blocks)array[z-1][y][x])(generator);

                        }
                    // This statement counts on adjacentcy to get the last block, literally previous item in the row
                    }else if(x!=0){
                        array[z][y][x]=setBlockDistribution(z,(blocks)array[z][y][x-1],Empty,(blocks)array[z-1][y][x])(generator);
                    // This statement is default, if no blocks precede this coming block, it is treated as
                    //  Empty(0) preceding the coming block
                    }else{
                        array[z][y][x]=setBlockDistribution(z,Empty,Empty,(blocks)array[z-1][y][x])(generator);
                    }
                }else{
                    // This statement sets the Player(8) block on the second "level" or depth(1) of the level
                    //  It centers the player as much as possible
                    if((z==1)&&(y==HEIGHT/2)&&(x==WIDTH/2)){
                        array[z][y][x]=8;
                    }else{
                        array[z][y][x]=0;
                    }
                }

            }
        }
    }


}
void displayDepth(int chosenDepth,int array[DEPTH][HEIGHT][WIDTH]){
    if (chosenDepth<1){
        chosenDepth=1;
    }
    else if(chosenDepth>DEPTH){
        chosenDepth=DEPTH;
    }
    for(int i = 0;i<DEPTH;i++){
        if(chosenDepth==i+1){
            for(int i=0;i<HEIGHT;i++){
                std::cout<<"-";
            }
        }else{
            for(int i=0;i<HEIGHT;i++){
                std::cout<<"*";
            }
        }
        std::cout<<std::endl;
    }
    std::cout<<std::endl<<std::endl;

    for(int y=0;y<HEIGHT;y++){
        for(int x=0;x<WIDTH;x++){
            std::cout<<array[chosenDepth-1][y][x];
        }
        std::cout<<std::endl;
    }


}
void displayDepthCURSES(int chosenDepth,int array[DEPTH][HEIGHT][WIDTH]){
    int xSize,ySize;
    getmaxyx(stdscr,xSize,ySize);		/* get the number of rows and columns */
    // mvprintw(0,0,"%s",mesg);
    // mvprintw(1,1,"%s",mesg);

    if (chosenDepth<1){
        chosenDepth=1;
    }
    else if(chosenDepth>DEPTH){
        chosenDepth=DEPTH;
    }
    // printw("Try resizing your window(if possible) and then run this program again\n");
    for(int y = 0;y<DEPTH;y++){
        for(int z=0;z<HEIGHT;z++){
            if(chosenDepth==y+1){
                mvprintw(y+1,z+1,"%s","-");
            }else{
                mvprintw(y+1,z+1,"%s","*");

            }
            
        }
    }

    for(int y=0;y<HEIGHT;y++){
        for(int x=0;x<WIDTH;x++){
            mvprintw(y+1,x+HEIGHT+10,"%i",array[chosenDepth-1][y][x]);
        }
    }
    refresh();


}
void displayPlayerInfoCURSES(PlayerObj Steve){
    mvprintw(DEPTH-10,HEIGHT+10,"%s %i","yPos: ",Steve.yPos);
    mvprintw(DEPTH-9,HEIGHT+10,"%s %i","xPos: ",Steve.xPos);
    mvprintw(DEPTH-8,HEIGHT+10,"%s %i","zPos: ",Steve.zPos);
    mvprintw(DEPTH-7,HEIGHT+10,"%s %i","Block under: ",Steve.standingBlock);


}
void exportWorldToFile(int array[DEPTH][HEIGHT][WIDTH]){
    for(int depth=0;depth<DEPTH;depth++){
        std::ofstream layerFile;
        layerFile.open (std::to_string(depth)+".dat");
        for(int height=0;height<HEIGHT;height++){
            for(int width=0;width<WIDTH;width++){
                layerFile << std::to_string(array[depth][height][width])<<std::endl;
            }
        }
        layerFile.close();
    }
}
// Assumes that Player(8) is unique
void findPlayer(PlayerObj &Steve,int array[DEPTH][HEIGHT][WIDTH]){
    for(int depth=0;depth<DEPTH;depth++){
        for(int height=0;height<HEIGHT;height++){
            for(int width=0;width<WIDTH;width++){
                if(array[depth][height][width]==8){
                    Steve.xPos=width;
                    Steve.zPos=height;
                    Steve.yPos=depth;
                    return;
                }
            }
        }
    }
}
void shiftColumnDown(int y,int z,int x, int array[DEPTH][HEIGHT][WIDTH]){
    if(array[y][z][x]==0){
        for(;y>0;y--){
            if(array[y][z][x]==0){
                array[y][z][x]=array[y-1][z][x];
                array[y-1][z][x]=0;
            }
        }
    }

}
void gravity(int array[DEPTH][HEIGHT][WIDTH]){
    for(int y=DEPTH-1;y>=0;y--){
        for(int z=0;z<HEIGHT;z++){
            for(int x=0;x<WIDTH;x++){
                if(array[y][z][x]==0){
                    shiftColumnDown(y,z,x,array);
                }
            }
        }
    }

}
void updatePlayer(PlayerObj &Steve,int array[DEPTH][HEIGHT][WIDTH]){
    int newXPos=Steve.xPos;
    int newYPos=Steve.yPos;
    int newZPos=Steve.zPos;
    blocks newBlockUnder=(blocks) array[newYPos][newZPos][newXPos];

    findPlayer(Steve,array);
    int oldXPos=Steve.xPos;
    int oldYPos=Steve.yPos;
    int oldZPos=Steve.zPos;
    blocks oldBlockUnder=Steve.standingBlock;

    array[oldYPos][oldZPos][oldXPos]=oldBlockUnder;


    array[newYPos][newZPos][newXPos]=Player;
    Steve.xPos= newXPos;
    Steve.yPos= newYPos;
    Steve.zPos= newZPos;
    Steve.standingBlock= newBlockUnder;


}
int main(){

    //width
    //    std::cout<<"Got to main";
    int xyzBlocks[DEPTH][HEIGHT][WIDTH];
    generateWorld(xyzBlocks);
    exportWorldToFile(xyzBlocks);



    PlayerObj Steve;
    findPlayer(Steve,xyzBlocks);
    
    char c;
    int depth=1;



    initscr();
    displayDepthCURSES(depth,xyzBlocks);
    displayPlayerInfoCURSES(Steve);
    
    cbreak();
    // noecho();

    while(1)
    {
        char c = getchar();

        switch(c){
            case 'u':
                if((depth-1)>=1){
                    depth-=1;
                    // system("clear");
                 }
                break;
            case 'l':
                if((depth+1)<=DEPTH){
                    depth+=1;
                    // system("clear");
                }
                break;
            case 'w':
                if((Steve.zPos-1)>=0){
                    Steve.zPos=Steve.zPos-1;
                    // system("clear");
                }
                break;
            case 's':
                if((Steve.zPos+1)<HEIGHT){
                    Steve.zPos=Steve.zPos+1;
                    // system("clear");
                }
                break;
            case 'a':
                if((Steve.xPos-1)>=0){
                    Steve.xPos=Steve.xPos-1;
                    // system("clear");
                }
                break;
            case 'd':
                if((Steve.xPos+1)<WIDTH){
                    Steve.xPos=Steve.xPos+1;
                    // system("clear");
                }
                break;
            case 'r':
                if((Steve.yPos-1)>=0){
                    Steve.yPos=Steve.yPos-1;
                    // system("clear");
                }
                break;
            case 'f':
                if((Steve.yPos+1)<DEPTH){
                    Steve.yPos=Steve.yPos+1;
                    // system("clear");
                }
                break;


        }
        gravity(xyzBlocks);
        updatePlayer(Steve,xyzBlocks);
        displayPlayerInfoCURSES(Steve);
        displayDepthCURSES(depth,xyzBlocks);

            // system("sleep 1");

    }
    

    






    //with config xBlocks[3], yBlocks[4], zBlocks[5]
    // ***  ***
    // ***  ***
    // ***  ***
    // ***  ***
    //      ***





    return 0;

}
