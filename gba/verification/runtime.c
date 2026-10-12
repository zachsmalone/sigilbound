#include <mgba/core/core.h>
#include <mgba-util/vfs.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

static struct mCore* core;
static const unsigned digits[10]={0x7B6F,0x2C97,0x62A7,0x628E,0x5BC9,0x798E,0x39EF,0x7292,0x7BEF,0x7BCE};
static void require(int ok, const char* label) {
    if (!ok) { fprintf(stderr, "FAIL: %s\n", label); exit(1); }
}
static void frames(int n) { while (n--) core->runFrame(core); }
static void press(unsigned key) {
    core->setKeys(core,0); frames(12);
    core->setKeys(core,key); frames(30);
    core->setKeys(core,0); frames(12);
}
static int matches(int x,int y,unsigned bits) {
    for (int row=0;row<5;++row) for (int col=0;col<3;++col) {
        unsigned actual=core->busRead16(core,0x06000000+2*((y+row*2)*240+x+col*2));
        unsigned expected=(bits & (1u<<(14-row*3-col))) ? (29u|(29u<<5)|(27u<<10)) : (3u|(5u<<5)|(9u<<10));
        if (actual!=expected) return 0;
    }
    return 1;
}
static int number(int x,int y,int value) {
    return matches(x,y,digits[value/10]) && matches(x+8,y,digits[value%10]);
}
static void state(int hp,int enemy,int attack,int brace,int xp,const char* label) {
    require(number(48,88,hp) && number(172,88,enemy) && number(64,132,attack) && number(128,132,brace) && number(136,116,xp),label);
    printf("PASS: %s\n",label); fflush(stdout);
}
static void shot(const char* path) {
    struct VFile* file=VFileOpen(path,O_WRONLY|O_CREAT|O_TRUNC);
    require(file!=NULL,"open screenshot");
    int ok=mCoreTakeScreenshotVF(core,file); file->close(file);
    require(ok,"save rendered screenshot");
}
int main(int argc,char** argv) {
    require(argc==2,"one ROM argument required");
    core=mCoreFind(argv[1]); require(core!=NULL,"identify ROM");
    require(core->init(core),"initialize mGBA");
    mCoreInitConfig(core,"sigilbound-verification");
    mCoreConfigSetDefaultIntValue(&core->config,"useBios",0);
    mCoreConfigSetDefaultIntValue(&core->config,"skipBios",1);
    core->loadConfig(core,&core->config);
    void* buffer=calloc(240*160,4); require(buffer!=NULL,"allocate video buffer");
    core->setVideoBuffer(core,buffer,240);
    require(mCoreLoadFile(core,argv[1]),"load ROM");
    core->reset(core); frames(600);
    require(core->busRead16(core,0x04000000)==0x1443,"battle display configuration");
    state(24,18,35,20,0,"boot and initial displayed counters");
    shot("evidence/startup.png");
    core->setKeys(core,1); frames(30);
    state(22,15,34,20,0,"single attack");
    frames(120); state(22,15,34,20,0,"held A does not repeat");
    core->setKeys(core,0); frames(12);
    press(8); state(24,18,35,20,0,"Start resets battle");
    press(2); state(23,18,35,19,0,"Brace reduces retaliation");
    press(8);
    for(int i=0;i<6;++i) press(1);
    state(14,0,29,20,20,"victory counters and reward");
    require(matches(12,116,0x5B6A),"victory status initial V");
    shot("evidence/victory.png");
    press(1); state(14,0,29,20,20,"victory reward not repeated");
    press(8);
    for(int i=0;i<20;++i) press(2);
    state(4,18,35,0,0,"Brace exhaustion");
    press(2); state(4,18,35,0,0,"unavailable Brace does not advance turn");
    press(1); press(1);
    state(0,12,33,0,0,"defeat counters without reward");
    require(matches(12,116,0x6B6E),"defeat status initial D");
    shot("evidence/defeat.png");
    press(8); state(24,18,35,20,0,"reset after defeat");
    printf("Behavioral framebuffer-memory checks passed. Screenshots require visual review; Delta remains untested.\n");
    mCoreConfigDeinit(&core->config); core->deinit(core); free(buffer);
    return 0;
}
