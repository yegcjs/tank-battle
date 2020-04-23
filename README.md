# 高程课设2 坦克大战

* [坦克大战](#坦克大战)
  * [需求分析](#需求分析)
  * [模块划分](#模块划分)
  * [控制流程](#控制流程)
  * [参数设计](#参数设计)
  * [图形设计](#图形设计)



![image-20200423214113365](https://github.com/yegcjs/tank-battle/raw/master/img/image-20200423214113365.png)



## 需求分析

需要模仿游戏坦克大战，游戏中包含以下元素

* 地图
* 障碍物/特殊方格
* 坦克
* 子弹
* 道具

### 地图

* 地图是一个矩形，$\mbox{Height}\times\mbox{Width}$。
* 考虑`console`中行列的长宽比，地图中一个块占`consolo`一行两列

地图中块的坐标与二维数组下表一致，地图坐标系如下



![image-20200423235346909](https://github.com/yegcjs/tank-battle/raw/master/img/image-20200423235346909.png)

![](https://github.com/yegcjs/tank-battle/raw/master/img/image-20200423235123910.png地图中包含敌方生成点、玩家复活点、基地。

敌方坦克进入基地即为游戏失败。

### 障碍物/特殊方格

* 每个障碍物/特殊方格占据地图上一格
* 不同障碍物/特殊方格有不同防御力
  * 障碍物为防御力为**正**，如砖块、铁块
  * 非障碍物的特殊方格防御力为**负**，如草地
* 坦克无法通过有障碍物的格子
* 坦克或子弹在非障碍物特殊方格中根据类型不同由不同效果（如草地为隐身）
* 子弹可以破坏障碍物如果子弹攻击力大于障碍物防御力

### 坦克

* 一个坦克占据地图上一个九宫格
* 坦克可以在地图上上下左右移动、向前发射子弹
* 坦克有移动速度、生命值，发射的子弹有一定的攻击力（统称为坦克属性）
* 移动速度由该坦克的刷新时间间隔决定



* 分为玩家坦克和敌方坦克

  * 玩家坦克由玩家控制

    * 默认攻击力为$1$
    * 默认初始生命值1
    * 默认刷新时间间隔为4个单位时间

  * 敌方坦克由电脑控制

    * 敌方坦克包括普通坦克、轻装坦克、重装坦克、装甲车等

    * 不同坦克的区别在于坦克的属性不同

    * 每隔一定时间在特定敌军产生点随机产生不同类型敌军坦克

    * 敌军坦克每一行动中不同行为（移动、射击）的概率

      

### 子弹

* 一个子弹占据地图上一格
* 子弹有固定的移动方向、刷新时间间隔、攻击力
* 子弹被发射后，属性不会发生改变
* 子弹与其他元素相遇，有如下三种情况
  * 子弹与子弹：两子弹均消失
  * 子弹与坦克：子弹消失，减少坦克相等于子弹攻击力的HP
  * 子弹与障碍物：子弹消失，如果障碍物防御力低于子弹攻击力，障碍物消失



### 道具

道具可以为坦克提供增益效果，需要考虑

* 增益内容与还原
* 增益结束时间
* 增益对象
* 每击杀一定数量的敌方坦克，在地图上空白格随机产生一个可供坦克拾取的道具

考虑以下四种增益：

* 增加攻击力
* 增加HP
* 一段时间内免伤
* 增加移动速度（减少刷新间隔）





### 模块划分

考虑以下几点

1. 障碍物、坦克、子弹、道具均可以视作地图上的元素，聚集在地图类中
2. 一个元素的位置、和物体运动的方向需要通过一个二维向量来记录
3. 坦克、子弹均具有位置、移动的功能和属性。
4. 玩家坦克和敌军坦克行为更新方式不同

	* 玩家坦克由键盘读入控制
	* 敌军坦克通过随机数按概率控制
5. 道具有两个阶段
   * 先在地图上某个方格出现
   * 被某个坦克拾取后附着在坦克上



![image-20200423121401944](https://github.com/yegcjs/tank-battle/raw/master/img/image-20200423121401944.png)

### 全局函数/变量

![image-20200423203341170](https://github.com/yegcjs/tank-battle/raw/master/img/image-20200423203341170.png)

### 成员

start: 开始界面

gaming: 游戏过程

end: 结束界面

### 变量

`cur_time`:当前时间（gaming中每经历一次主循环，cur_time++)

### xy

一个二维向量，重载加减运算

![image-20200423122356012](https://github.com/yegcjs/tank-battle/raw/master/img/image-20200423122356012.png)

### Map

![image-20200423121535934](https://github.com/yegcjs/tank-battle/raw/master/img/image-20200423121535934.png)

#### 方法

* Map: 构造函数 
  * 读取地图、地图配置文件
  * 生成一个玩家
  * 初始化容器和各个地图标记
  * 生成一个painter，绘制屏幕
* ~Map:析构函数
  * 消亡所有子弹、坦克
  * 消亡painter
* reset_player：
  * 如果当前有`player`，则消亡并赋为`NULL`
  * 判断当前是否能够在玩家复活点放置`player`，若可以则放置
* set_enemy_act:
  * 根据一定概率，给每个到刷新时间的敌方坦克设置动作
* set_player_act:
  * 玩家刷新时间到，则读取键盘按键信息为玩家坦克设置动作
* tank_OK(xy pos):
  * 判断能否在`pos`放置一个坦克
* add_buff:
  * 判断当前是否符合产生一个新Buff在地图上的条件
  * 若符合，则产生
* add_enemy:
  * 判断当前是否符合产生一个新敌军坦克的条件
  * 若符合，则根据一定概率设置参数，产生坦克
* update:
  * 更新当前步坦克、子弹状态下，坦克、子弹的新位置
  * 处理坦克、子弹之间的相互攻击等作用

#### 成员

* blocks: Block[HEIGHT]\[WIDHT]，表示某个位置上的障碍物/特殊方块

  其中 Block为，枚举类的值对应防御力，empty即无障碍物、无特殊方块

  ![image-20200423121655699](https://github.com/yegcjs/tank-battle/raw/master/img/image-20200423121655699.png)

* cur_tanks, cur_bullets, cur_buff与blocks类似

  * cur_tanks: Tank*[HEIGHT]\[WIDTH]

  * cur_bullets: Bullet*[HEIGHT]\[WIDHT]

  * cur_buff: buff_type[HEIGHT]\[WIDHT]

    ​	![image-20200423121910321](https://github.com/yegcjs/tank-battle/raw/master/img/image-20200423121910321.png)

* buffs:  set<Buff*> 表示当前存在在坦克上的Buff

* bullets, enemies, mybullets与buffs类似

  * bullets: set<Bullet*>
  * enemies: set<Enemy*>
  * mybullets: set<Bullet*>

* enemy_spown_point: vector\<xy\> 表示敌军生成点

* player_revive_point: xy 玩家复活点

* player: *Player 玩家坦克

* painter: Painter* 绘图类对象，用于擦除、绘制各种元素

* info: Struct CONFIG 游戏信息，包含目标击杀数、玩家初始HP，玩家已杀敌数，玩家剩余命数

  ![image-20200423122736651](https://github.com/yegcjs/tank-battle/raw/master/img/image-20200423122736651.png)

### Buff

![image-20200424001817741](https://github.com/yegcjs/tank-battle/raw/master/img/image-20200424001817741.png)

#### 方法

* Buff(Tank*, type): 初始化对象的所有成员
* ~Buff(): buff结束，对拥有buff的坦克进行还原
* timeout() 判断是否到buff结束时间
* get_type(): 返回buff类型
* get_owner(): 返回拥有该buff的坦克指针

#### 成员

* end_time: buff结束时间
* myType: buff类型
* old_value: buff增益值的未增益前的值，如攻击力增加buff：则为增加攻击力前的攻击力
* owner: buff附着在的坦克

### Object

表示可移动物体

![image-20200423202844739](https://github.com/yegcjs/tank-battle/raw/master/img/image-20200423202844739.png)

### 方法

Object: 构造函数，初始化初始位置、运动方向、创建时间、刷新时间间隔

direction: 获取当前方向，返回一个当前运动方向

get_interval: 返回物体每次移动的间隔

move: 物体向运动方向移动一个格

set_direction: 改变物体运动方向

set_interval: 该变物体移动间隔时间

start_time: 返回物体创建时间

timeup: 返回该物体是否到了可以移动的时间

#### 成员

* dir: xy 表示运动方向，上下左右分别为$(-1,0),(1,0)，(0,-1),(0,1)$

* pos: xy 表示物体所在的坐标

* interval: int 表示刷新时间间隔（越小速度越大，子弹为最大的1）

* time: int 物体创建时间

### Bullet

![image-20200423204043711](https://github.com/yegcjs/tank-battle/raw/master/img/image-20200423204043711.png)

#### 方法

* Bullet: 初始化Object基类，初始化攻击力
* get_attack: 返回子弹的攻击力

### 成员

* atk: 攻击力



### Tank

![image-20200423204226431](https://github.com/yegcjs/tank-battle/raw/master/img/image-20200423204226431.png)

#### 方法

* Tank: 构造函数，初始化Object基类、攻击力、第一步动作、生命值，并设定为不在射击状态
* get_attack: 返回攻击力
* attacked（int atc): 表示坦克受到了攻击，返回剩余生命值
* isShooting: 返回当前坦克是否在射击状态
* next_action: 返回该坦克将要进行的移动方向
* set_attack: 改变攻击力
* set_hp: 改变生命值
* stopShooting: 停止射击状态

#### 成员

* atcn: Action枚举类型，下一步动作

  ![image-20200423204611002](https://github.com/yegcjs/tank-battle/raw/master/img/image-20200423204611002.png)

* atk: int 发出子弹的攻击力
* hp: int 生命值
* shooting: bool 是否在射击状态

## Enemy / Player

![image-20200423204757171](https://github.com/yegcjs/tank-battle/raw/master/img/image-20200423204757171.png)	![image-20200423204802539](https://github.com/yegcjs/tank-battle/raw/master/img/image-20200423204802539.png)

 区别仅在于act方法，即设置下一步动作方式不同

* Enemy::act() 随机产生
* Player::act(Action) 根据玩家给出的动作设置



## Painter

绘制各个元素

![image-20200423205008683](https://github.com/yegcjs/tank-battle/raw/master/img/image-20200423205008683.png)

#### 方法

* Painter: 初始化，读取图标文件，若无则定为"??"
* locate(xy): 移动光标到给定坐标，其中两宽度等于一高度，并考虑左边框和右边框，对应加一
* paint_info: 绘制得分、目标栏
* paint_map: 绘制整个地图所有block
* paint_frame: 绘制地图外框
* paint_block/bullet/tank/buff: 绘制一个元素
* depaint_tank: 将整个坦克所在区域全部清除为原来的block

#### 成员

* block_icon: map<Block, string>
* buff_icon: map<buff_type, string> 
* bullet_icon: string 子弹图标（只有一种）
* tank_icon: string[5] 对应坦克移动速度



## 控制流程

游戏整体运行流程如下

1. 开始界面，选择关卡

2. 进入游戏，

   1. 新建一个Map对象`game`，构造函数完成以下内容

      ​	读取配置文件`map_config.txt`，地图文件`mapi.csv`

   2. 根据配置文件初始化玩家坦克、初始化地图障碍物和特殊方格分布

   3. 初始化地图上坦克、道具、子弹分布的记录（道具子弹全为空，坦克仅有玩家坦克）

   4. 初始化绘画器

3. 调用`paint()`绘制屏幕进入游戏画面

4. 进入游戏主循环

   1. 更新敌方坦克行动`set_enemy_act()`、更新玩家行动`set_player_act()`

   2. 执行坦克、子弹的动作，并处理执行动作带来的效果：

      1. 执行坦克动作

         1. 判断坦克是否到刷新时间，若没有则不发生变化

            （某个$\mbox{object}$到刷新时间$\Leftrightarrow$$\mbox{interval}\mid(\mbox{cur_time}-\mbox{obj.time})$

         2. 坦克移动

            1. 移动后将离开地图：无法移动
               
            2. 撞到障碍物或者其他坦克：无法移动
            
               * 注：撞到其他坦克的判断方式
            
         * 在遍历坦克前，清空坦克`cur_tank`,所有坦克先来后到更新到`cur_tank`
           
           > Tank *cur_tank\[i]\[j]表示(i,j)方格上的坦克
           
         * 则如果两个坦克在下一个时间移动后处于同一位置，遍历顺序后的无法移动到该位置
           
            3. 坦克移动后，占据的九宫格内有Buff
            
               * 创建一个新的`Buff`对象，赋予给该坦克，并加入到容器中
            
               * `cur_buff`中清除这一`Buff`
            
                 （`cur_buff`中记录在地图某个位置和`Buff`对应道具这一元素的先后两个阶段）
                 
                 > Buff: buff_type cur_buff\[i]\[j] 表示 (i,j)方格上所存在的buff
            
          3.   若坦克处于射击状态
                     * 创建一个新的`Bullet`对象，加入到容器中
                           * 如果是玩家坦克发出子弹，则额外加入到玩家发出子弹的容器中
         
      2. 执行子弹动作

         1. 判断子弹是否到刷新时间，若没有则不发生变化

         2. 子弹移动

            1. 移动后离开地图：从容器移除、消亡

            2. 与障碍物、坦克、其他子弹相碰

               * 根据攻击力、防御力、HP的规则处理
               * 将需要消亡的对象从容器中erase，消亡相应对象，修改`cur_tank`,`cur_bullet`,`blocks`对应标记

               > Bullet* cur_bullet\[i]\[j] 表示(i,j)方格上的子弹

               * 如果是玩家坦克被击杀：命数-1，尝试`reset_player()`

                 （若复活点被其他坦克占据，标记`player=NULL`，下一时间再尝试`reset_player()`）

            3. 修改`cur_bullet`的标记（清除旧位置、更新新位置）

      【以上1.2步均会在更新位置的同时进行重绘】

      执行子弹动作后，如果玩家命数耗尽：返回游戏失败；如果玩家击杀敌方坦克数量达标：返回游戏胜利

    3. 如果游戏胜利，跳转至结束界面，否则进行4，5

    4. 尝试新增敌方坦克（如果时间到、且敌方生成点未被全占据则产生）

    5. 尝试在地图上新增道具（如果击杀敌方数达到一定数额，产生）

5. 结束界面，选择返回开始界面或退出



### 参数设计

* 地图大小：$30\times30$

* 玩家坦克默认属性：命数3，HP1，攻击力1

* 单位时间间隔的时间长度（仅计主循环一次后Sleep的时间）：50ms

* 敌方生成点：每个生成点等概率，被坦克占据的点的概率转移到后一个点

* 敌方坦克属性：攻击力1或2（80%,20%），生命值1-3等概率，移动间隔2或4等概率

* 敌方坦克各个行动概率：15%不改变动作，20%上下左右（不包含不改变），5%射击（不含不改变）【即上一步为向上，则这一步向上概率为35%】

* Buff参数
  
  * 产生条件：没击杀5个敌军产生一个buff，位置随机
  * 产生种类：无敌、增加HP（永久）、增加攻击力、加速概率相同
  * 持续时间：$100\ln X$，$X$为已击杀的敌军（以拾取时为准）
  
  

### 图形设计

* 障碍物/特殊方格
  * ![image-20200423235435684](https://github.com/yegcjs/tank-battle/raw/master/img/image-20200423235435684.png)砖块，防御力1
  * ![image-20200423235454119](https://github.com/yegcjs/tank-battle/raw/master/img/image-20200423235454119.png)铁块，防御力2
  * ![image-20200423235503920](https://github.com/yegcjs/tank-battle/raw/master/img/image-20200423235503920.png)草地
  * ![image-20200423235511117](https://github.com/yegcjs/tank-battle/raw/master/img/image-20200423235511117.png)基地
* 子弹
  * ![image-20200423235526599](https://github.com/yegcjs/tank-battle/raw/master/img/image-20200423235526599.png)
* 坦克
  * 颜色：由生命值决定
    * 绿色：1
    * 蓝色：2
    * 紫色：3或以上
  * 图标：由刷新间隔决定
    * 圆![image-20200423235707681](https://github.com/yegcjs/tank-battle/raw/master/img/image-20200423235707681.png)：4及4以上
    * 方块![image-20200423235725748](https://github.com/yegcjs/tank-battle/raw/master/img/image-20200423235725748.png)：4以下
  * ![image-20200423235932113](https://github.com/yegcjs/tank-battle/raw/master/img/image-20200423235932113.png)

* 道具
  * ![image-20200424000049150](https://github.com/yegcjs/tank-battle/raw/master/img/image-20200424000049150.png)：加速
  * ![image-20200424000101633](https://github.com/yegcjs/tank-battle/raw/master/img/image-20200424000101633.png)：增加生命值
  * ![image-20200424000112087](https://github.com/yegcjs/tank-battle/raw/master/img/image-20200424000112087.png)：免伤
  * ![image-20200424000124930](https://github.com/yegcjs/tank-battle/raw/master/img/image-20200424000124930.png)：增加攻击力

![image-20200424000936073](https://github.com/yegcjs/tank-battle/raw/master/img/image-20200424000936073.png)