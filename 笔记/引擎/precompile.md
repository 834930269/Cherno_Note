# 预编译头

当我们编译C的时候,如果不是用这个技术就需要全局编译,但事实上有一些库我们完全可以编译一次后就不再管它  
比如常见的STL库  

所以就引出了`预编译头`  
即有些文件我们可以对其进行预编译,`但是每个CPP都需要引用这个.h`  
预编译文件在之后就不需要再次编译了  

## premake5中实现预编译

> 在对应的项目中填入下面的代码

```lua
-- 全局的预编译头文件
pchheader "hzpch.h"
pchsource "Hazel/src/hzpch.cpp"
```

其中`hzpch.h`的内容如下:  

```h
#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>


#ifdef HZ_PLATFORM_WINDOWS
	#include <Windows.h>
#endif

```

即一开始会将这些进行预编译,预编译完成后,下次再对项目进行编译就不需要编译这些了,速度会快很多



