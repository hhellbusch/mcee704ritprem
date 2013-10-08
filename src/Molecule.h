#pragma once

#ifdef ENABLE_MEMWATCH
      #include <MemWatch.h>
      #define new    DEBUG_NEW
#endif	// ENABLE_MEMWATCH