#include "common.h"
#include <inttypes.h>

void mem_read(uintptr_t block_num, uint8_t *buf);
void mem_write(uintptr_t block_num, const uint8_t *buf);

static uint64_t cycle_cnt = 0;

void cycle_increase(int n) { cycle_cnt += n; }

//void srand (unsigned int seed);

#define MAX_way 16//路数最多为16

#define MAX_group 100000//最多组数

uint32_t wnum=0;//路数

uint32_t gnum=0;//组数

typedef struct
{
 
  bool valid;
  bool dirty;
  uint32_t tag;
  uint8_t data[64];

} line;

typedef line* group;

typedef group* cache;


cache myC;

uint32_t tlen;

uint32_t glen;//组号长度

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
  
  //assert(!(addr>>20));
  //assert(exp2(addr)<=MEM_SIZE);

  addr=addr&(~0x3);

  uint32_t g=(addr>>BLOCK_WIDTH)&mask_with_len(glen);
  uint32_t offset=addr&mask_with_len(BLOCK_WIDTH);
  uint32_t tag=(addr>>(BLOCK_WIDTH+glen))& mask_with_len(tlen);

  
  //hit
  for(int i=0;i<wnum;i++){
    if(myC[g][i].valid==true&&(myC[g][i].tag==tag))
    {
      //assert(0);
      uint32_t ans=0;
      
      for(int w=3;w>=0;w--){
        ans<<=8;
        ans+=myC[g][i].data[offset+w];
      }
      return ans;
    }
  }
  //miss

  for(int i=0;i<wnum;i++){
    //assert(0);
    //printf("%d\n",cache[g].valid[i]);
    if(!myC[g][i].valid){
      
      myC[g][i].valid=1;
      mem_read(addr>>BLOCK_WIDTH,myC[g][i].data);
      myC[g][i].tag=tag;
      //assert(0);
      uint32_t ans=0;
      
      for(int w=3;w>=0;w--){
        ans<<=8;
        ans+=myC[g][i].data[offset+w];
        
      }
      //assert(0);
      return ans;
      //assert(0);
    }
  }
  //还满了
  int lucker=rand()%wnum;
  if(myC[g][lucker].dirty==1)mem_write((myC[g][lucker].tag<<glen)+g,myC[g][lucker].data);//写回操作
  mem_read(addr>>BLOCK_WIDTH,myC[g][lucker].data);
  myC[g][lucker].tag=tag;

  uint32_t ans=0;

      for(int w=3;w>=0;w--){
        ans<<=8;
        ans+=myC[g][lucker].data[offset+w];
      }
      myC[g][lucker].dirty=0;
      //assert(0);
      return ans;

  //return 0;
}
// 往 cache 中 addr 地址所属的块写入数据 data，写掩码为 wmask
// 例如当 wmask 为 0xff 时，只写入低8比特
// 若缺失，需要从先内存中读入数据
void cache_write(uintptr_t addr, uint32_t data, uint32_t wmask) {
  addr=addr&~0x3;
  uint32_t g=(addr>>BLOCK_WIDTH)&mask_with_len(glen);
  uint32_t offset=addr&mask_with_len(BLOCK_WIDTH);
  assert( ~(~0 << BLOCK_WIDTH)==mask_with_len(BLOCK_WIDTH));
  uint32_t tag=(addr>>(BLOCK_WIDTH+glen))& mask_with_len(tlen);
  //找到了
  //assert(0);
  
	for(int i=0;i<wnum;i++){
    //printf("%d",cache[g].valid[i]);
   //assert(0);
    		if(myC[g][i].valid&&(myC[g][i].tag==tag))
        { 
          //assert(0);
          myC[g][i].dirty=true;
          //uint32_t rnum=(data&wmask);
          //uint32_t  j=addr&mask_with_len(BLOCK_WIDTH);
          //先当是按照单元来的
          uint32_t* p=(uint32_t*)&myC[g][i].data[offset];
          *p=(*p&~wmask)|(data&wmask);
          return;
        }
  }
  //缺失
  for(int i=0;i<wnum;i++){
    if(!myC[g][i].valid){
      myC[g][i].valid=true;
      mem_read(addr>>BLOCK_WIDTH,myC[g][i].data);
      myC[g][i].tag=tag;
      myC[g][i].dirty=true;

//assert(0);
      //uint32_t rnum=(data&wmask);
      //uint32_t  j=addr&mask_with_len(BLOCK_WIDTH);
          //先当是按照单元来的
          uint32_t* p=(uint32_t*)&myC[g][i].data[offset];
          *p=(*p&~wmask)|(data&wmask);
         return;
         
    }
  }
  //assert(0);
  int lucker=rand()%wnum;
  if(myC[g][lucker].dirty)mem_write((myC[g][lucker].tag<<glen)+g,myC[g][lucker].data);
  mem_read(addr>>BLOCK_WIDTH,myC[g][lucker].data);
  myC[g][lucker].dirty=true;
  myC[g][lucker].valid=true;
  myC[g][lucker].tag=tag;

  //先当是按照单元来的
  uint32_t* p=(uint32_t*)&myC[g][lucker].data[offset];
  *p=(*p&~wmask)|(data&wmask);
  
  //assert(0);

  
}
// 初始化一个数据大小为 2^total_size_width B，关联度为 2^associativity_width 的 cache
// 例如 init_cache(14, 2) 将初始化一个 16KB，4 路组相联的cache
// 将所有 valid bit 置为无效即可
void init_cache(int total_size_width, int associativity_width) {
  //assert((exp2(associativity_width))<MAX_way);
  //assert((uint64_t)(1<<total_size_width)<(uint64_t)(MAX_group<<6)*(uint64_t)(1<<associativity_width));
  
  wnum=exp2(associativity_width);//路数

  glen=total_size_width-BLOCK_WIDTH-associativity_width;

  gnum=exp2(glen);//（组数=总空间/路数/64B）//先不考虑不整除；

  tlen=32-glen-BLOCK_WIDTH;//tag长度

  //printf("%d\n",glen);

  //srand(time(NULL));
  myC=(cache)malloc(gnum*sizeof(group));

  for(int i=0;i<gnum;i++){
    myC[i]=(group)malloc(wnum*sizeof(line));
    for(int j=0;j<wnum;j++){
      myC[i][j].valid=false;
      myC[i][j].dirty=false;
    }

  }
  
}



void display_statistic(void) {
}
