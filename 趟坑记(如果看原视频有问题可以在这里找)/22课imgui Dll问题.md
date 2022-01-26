这里会报三个LINKError 2019  
即无法找到外部库的那三个函数符号  

Cherno给出的解释是  
因为Hazel是动态dll,而imgui是静态lib  
而Sandbox只连接了Hazel项目  
(所以应该是dll中连接的时候因为没有那三个符号,导致加载静态库时没有加载到内存里,或者编译中**这里有点不熟悉,后面系统学习编译以后回来补**)    

Hazel给出的暂时的解决方案是:  

在Hazel项目预编译头加上`IMGUI_API=__declspec(dllexport);`  
ImGui项目预编译头加上`IMGUI_API=__declspec(dllexport);`  
Sandbox项目预编译头加上`IMGUI_API=__declspec(dllimport);`

然后在Hazel的一个可执行CPP中加入这三个函数的调用  
```cpp
ImGui::Begin("Test");
ImGui::Text("Test");
ImGui::End();
```

然后生成项目,之后把Hazel项目中的这三行代码删掉即可

大概意思是让dll能够编译进这三个符号


# 静态库解决方法

后面的课Cherno直接把Hazel改成了静态库,即直接一次性加载也就规避了动态库的问题  
也不用像上面一样编译那么麻烦
但是要注意的是,编译静态库需要修改imgui,导致无法和远端submodule同步,所以这里的项目直接把imgui变成本地的了