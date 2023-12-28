#include "common.h"
#include <inttypes.h>

void mem_read(uintptr_t block_num, uint8_t *buf);
void mem_write(uintptr_t block_num, const uint8_t *buf);

/*2的16次方，4路组相连*/

static uint64_t cycle_cnt = 0;

void cycle_increase(int n) { cycle_cnt += n; }

//void srand (unsigned int seed);

#define MAX_way 16//路数最多为16

#define MAX_group 100000//最多组数

uint32_t wnum=0;//路数

uint32_t gnum=0;//组数

typedef struct
{
  /* data */
  bool valid;
  bool dirty;
  uint32_t tag;
  uint8_t data[64];
} line;

typedef struct
{
  /* data */
  line* ways;

}group;

typedef struct 
{
  /* data */
  group* groups;
}cache;

cache myC;

//struct{
//  bool valid[16];//是否有效
 // bool dirty[16];
 // uint32_t tag[16];//标志位
 // uint8_t data[16][64];//数据
//}cache[10000];
//定义cache

// TODO: implement the following functions

// 从 cache 中读出 addr 地址处的 4 字节数据
// 若缺失，需要先从内存中读入数据

//32位地址

uint32_t cache_read(uintptr_t addr) {
  
  //assert(exp2(addr)<=MEM_SIZE);

  uint32_t g=(addr>>6)%gnum;
  //printf("%d\n",gnum);
  //printf("%d\n",g);
  //命中
  //assert(0);
  for(uint32_t i=0;i<wnum;i++){
    if(myC.groups[g].ways[i].valid==true&&(myC.groups[g].ways[i].tag==((addr>>6)/gnum)))
    {
      //assert(0);
      uint32_t ans=0;
      uint32_t a=addr%BLOCK_SIZE;
      for(int w=3;w>=0;w--){
        ans=(ans<<8)+myC.groups[g].ways[i].data[a+w];
      }
      return ans;
    }
  }
  //缺失
  for(uint32_t i=0;i<wnum;i++){
    //assert(0);
    //printf("%d\n",cache[g].valid[i]);
    if(!myC.groups[g].ways[i].valid){
      myC.groups[g].ways[i].valid=true;
      mem_read(addr>>6,myC.groups[g].ways[i].data);
      myC.groups[g].ways[i].tag=(addr>>6)/gnum;
      //assert(0);
      uint32_t ans=0;
      uint32_t a=addr%BLOCK_SIZE;
      for(int w=3;w>=0;w--){
        ans=(ans<<8)+myC.groups[g].ways[i].data[a+w];
      }
      return ans;
      //assert(0);
    }
  }
  //还满了
  int lucker=rand()%wnum;
  if(myC.groups[g].ways[lucker].dirty)mem_write(myC.groups[g].ways[lucker].tag*gnum+g,myC.groups[g].ways[lucker].data);//写回操作
  mem_read(addr>>6,myC.groups[g].ways[lucker].data);
  myC.groups[g].ways[lucker].tag=(addr>>6)/gnum;

  uint32_t ans=0;
      uint32_t a=addr%BLOCK_SIZE;
      for(int w=3;w>=0;w--){
        ans=(ans<<8)+myC.groups[g].ways[lucker].data[a+w];
      }
      //assert(0);
      return ans;

  //return 0;
}
// 往 cache 中 addr 地址所属的块写入数据 data，写掩码为 wmask
// 例如当 wmask 为 0xff 时，只写入低8比特
// 若缺失，需要从先内存中读入数据
void cache_write(uintptr_t addr, uint32_t data, uint32_t wmask) {
  uint32_t g=(addr>>6)%gnum;
  //找到了
  //assert(0);
  
	for(uint32_t i=0;i<wnum;i++){
    //printf("%d",cache[g].valid[i]);
   //assert(0);
    		if(myC.groups[g].ways[i].valid&&(myC.groups[g].ways[i].tag==((addr>>6)/gnum)))
        { 
          //assert(0);
          myC.groups[g].ways[i].dirty=true;
          //uint32_t rnum=(data&wmask);
          int  j=addr%BLOCK_SIZE;
          //先当是按照单元来的
          uint32_t* p=(uint32_t*)&myC.groups[g].ways[i].data[j];
          *p=(*p&~wmask)|(data&wmask);
          return;
        }
  }
  //缺失
  for(int i=0;i<wnum;i++){
    if(!myC.groups[g].ways[i].valid){
      myC.groups[g].ways[i].valid=true;
      mem_read(addr>>6,myC.groups[g].ways[i].data);
      myC.groups[g].ways[i].tag=(addr>>6)/gnum;
      myC.groups[g].ways[i].dirty=true;
assert(0);
      uint32_t rnum=(data&wmask);
      int  j=addr%BLOCK_SIZE;
          //先当是按照单元来的
          uint32_t* p=(uint32_t*)&myC.groups[g].ways[i].data[j];
          *p=(*p&~wmask)|(data&wmask);
         return;
         
    }
  }
  //assert(0);
  int lucker=rand()%wnum;
  mem_read(addr>>6,myC.groups[g].ways[lucker].data);
  myC.groups[g].ways[lucker].tag=(addr>>6)/gnum;
  uint32_t rnum=(data&wmask);
  int  j=addr%BLOCK_SIZE;
          //先当是按照单元来的
          uint32_t* p=(uint32_t*)&myC.groups[g].ways[lucker].data[j];
          *p=(*p&~wmask)|(data&wmask);
  //assert(0);

  
}
// 初始化一个数据大小为 2^total_size_width B，关联度为 2^associativity_width 的 cache
// 例如 init_cache(14, 2) 将初始化一个 16KB，4 路组相联的cache
// 将所有 valid bit 置为无效即可
void init_cache(int total_size_width, int associativity_width) {
  assert((exp2(associativity_width))<MAX_way);
  assert((uint64_t)(1<<total_size_width)<(uint64_t)(MAX_group<<6)*(uint64_t)(1<<associativity_width));
  
  wnum=exp2(associativity_width);//路数
  gnum=((uint64_t)(1<<(total_size_width-6))/wnum);//（组数=总空间/路数/64B）//先不考虑不整除；

  //printf("%d\n",total_size_width);

  //srand(time(NULL));
  myC.groups=(group*)malloc(gnum*sizeof(group));

  for(int i=0;i<gnum;i++){
    myC.groups[i].ways=(line*)malloc(wnum*sizeof(line));
    for(int j=0;j<wnum;j++){
      myC.groups[i].ways[j].valid=0;
      myC.groups[i].ways[j].dirty=0;
    }

  }
  
}



void display_statistic(void) {
}
