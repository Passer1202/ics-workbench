#include "common.h"
#include <inttypes.h>

void mem_read(uintptr_t block_num, uint8_t *buf);
void mem_write(uintptr_t block_num, const uint8_t *buf);

static uint64_t cycle_cnt = 0;

void cycle_increase(int n) { cycle_cnt += n; }

//void srand (unsigned int seed);

const int MAX_way=16;//路数最多为16

const int MAX_group=1e5;//最多组数

uint32_t wnum=0;//路数

uint32_t gnum=0;//组数

struct{
  bool valid[16];//是否有效
  uint16_t tag[MAX_way];//标志位
  uint8_t data[MAX_way][BLOCK_SIZE];//数据
}cache[MAX_group];
//定义cache



// TODO: implement the following functions

// 从 cache 中读出 addr 地址处的 4 字节数据
// 若缺失，需要先从内存中读入数据

//32位地址

uint32_t cache_read(uintptr_t addr) {
  
  assert(exp2(addr)<=MEM_SIZE);

  uint32_t g=(addr>>6)%gnum;

  //命中
  for(int i=0;i<wnum;i++){
    if(cache[g].valid[i]&&(cache[g].tag[i]==((addr>>6)/wnum)))
      return cache[g].data[i][addr%BLOCK_SIZE];
  }
  //缺失
  for(int i=0;i<wnum;i++){
    if(!cache[g].valid[i]){
      cache[g].valid[i]=true;
      mem_read(addr>>6,cache[g].data[i]);
      cache[g].tag[i]=(addr>>6)/wnum;
      return cache[g].data[i][addr%BLOCK_SIZE];
    }
  }
  //还满了
  int lucker=rand()%wnum;
  mem_read(addr>>6,cache[g].data[lucker]);
  cache[g].tag[lucker]=(addr>>6)/wnum;
  return cache[g].data[lucker][addr%BLOCK_SIZE];

  //return 0;
}
// 往 cache 中 addr 地址所属的块写入数据 data，写掩码为 wmask
// 例如当 wmask 为 0xff 时，只写入低8比特
// 若缺失，需要从先内存中读入数据
void cache_write(uintptr_t addr, uint32_t data, uint32_t wmask) {
}
// 初始化一个数据大小为 2^total_size_width B，关联度为 2^associativity_width 的 cache
// 例如 init_cache(14, 2) 将初始化一个 16KB，4 路组相联的cache
// 将所有 valid bit 置为无效即可
void init_cache(int total_size_width, int associativity_width) {
  assert((1<<associativity_width)<MAX_way);
  assert((uint64_t)(1<<total_size_width)<(uint64_t)(MAX_group<<6)*(uint64_t)(1<<associativity_width));
  
  wnum=1<<associativity_width;//路数
  gnum=((uint64_t)(1<<(total_size_width-6))/wnum);//（组数=总空间/路数/64B）//先不考虑不整除；


  srand(time(NULL));

  for(int i=0;i<gnum;i++){
    for(int j=0;j<wnum;j++){
      cache[i].valid[j]=false;
    }

  }
}

void display_statistic(void) {
}
