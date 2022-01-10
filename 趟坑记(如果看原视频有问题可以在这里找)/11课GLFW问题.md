> 如果直接拷贝cherno的`glfw`submodule的话  
会报`LINK 2019`的错误

这个错误的问题描述基本如下:  

```
unresolved __imp_strcnpy
```

之类的,意思是缺少了c运行时的库

主要问题来自这里:  
Cherno的最新的GLFW库的`premake文件中`  
**windows的环境下的system设置如下**

```lua
filter "system:windows"
		systemversion "latest"
```

这里缺少一个配置

```lua
filter "system:windows"
        buildoptions { "-std=c11", "-lgdi32" }
        systemversion "latest"
        staticruntime "On"
```

即`staticruntime "On"`

需要把c的静态运行时库加入进来,把这个更新一下就可以编译了...

但是如果直接修改submodule会无法上传,所以这里就直接把GLFW项目拷贝过来了.

感谢`Rory Becker`

https://www.youtube.com/watch?v=88dmtleVywk