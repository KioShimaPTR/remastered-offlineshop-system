// arat: #include "DragonSoul.h"
// altına ekle:
// BEGIN_OFFLINE_SHOP
#include "offlineshop_manager.h"
// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: #define IS_NO_CLEAR_ON_DEATH_AFFECT
// hepsini değiştir:
// BEGIN_OFFLINE_SHOP
#define IS_NO_CLEAR_ON_DEATH_AFFECT(type) ((type) == AFFECT_BLOCK_CHAT || (type) == AFFECT_SHOPOWNER || ((type) >= 500 && (type) < 600))
// END_OF_OFFLINE_SHOP