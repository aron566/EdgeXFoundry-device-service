@[toc](EdgeXFoundry的部署应用开发)
# EdgeX 历史记录和命名
EdgeXFoundry最初是戴尔物联网营销部特许的项目，由 CTO 戴尔客户办公室开发，于 2015 年 7 月作为名为"Fuse项目"的孵化项目开发。它最初创建为在戴尔的物联网网关介绍行上作为 IoT 软件应用程序运行。2017 年 4 月 24 日，戴尔通过 Linux 基金会将该项目引入开源。EdgeX 在 2017 年汉诺威展上正式宣布并展示。汉诺威博览会是世界上最大的工业贸易展览会之一。在展会上，Linux基金会还宣布了由50个创始成员组织（EdgeX生态系统）组成的协会，以帮助推进该项目和创建通用边缘平台的目标。
# 功能及流程简介
## EdgeX 的工作原理
[官网详细说明](https://docs.edgexfoundry.org/1.2/getting-started/)

说明中的一些名词定义：
**北向通讯**（一般指上报数据）
**南向通讯**（一般指下发命令或者数据）
### 传感器数据收集

EdgeX 的主要工作是从传感器和设备收集数据，使这些数据可供北端应用程序和系统使用。数据由表示该设备协议的设备服务从传感器收集数据。示例：Modbus 设备服务将在 Modbus 中进行通信，以便从 Modbus 泵设备获得压力读数。设备服务将传感器数据转换为 EdgeX 事件对象，并将事件对象通过 REST 通信（步骤 1）发送到核心数据服务。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20201103103631257.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80Mjg5MjEwMQ==,size_16,color_FFFFFF,t_70#pic_center)


核心数据将传感器数据保留在本地边缘数据库中。默认情况下，Redis 用作数据库（其他数据库可以替代使用）。事实上，持久性是不需要的，可以关闭。数据保留在边缘的 EdgeX 中，原因有两个：

- 边缘节点并不总是连接。在断开连接操作期间，必须保存传感器数据，以便恢复连接时可以向北传输。这称为存储和转发功能。
- 在某些情况下，传感器数据分析需要回顾历史，以便了解趋势，并基于该历史记录做出正确的决策。如果传感器报告现在温度为 72° F，则在决定调整加热或冷却系统之前，您可能想知道 10 分钟前的温度是多少。如果温度为 85° F，您可以决定调整以降低您十分钟前的室温足以冷却房间。历史数据的上下文对本地分析决策非常重要。

核心数据将传感器数据事件放在面向应用程序服务的消息主题上。默认情况下，ZeroMQ 用作消息传递基础结构（步骤 2）。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20201103103657433.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80Mjg5MjEwMQ==,size_16,color_FFFFFF,t_70#pic_center)


应用程序服务根据需要转换数据，并将数据推送到终结点。它还可以在将事件发送到终结点之前对事件进行筛选、丰富、压缩、加密或执行其他功能（步骤 3）。终结点可能是 HTTP/S 终结点、MQTT 主题、云系统（云主题）等。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20201103103716163.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80Mjg5MjEwMQ==,size_16,color_FFFFFF,t_70#pic_center)


### 边缘分析和驱动

在边缘计算中，简单地收集传感器数据只是 EdgeX 等边缘平台工作的一部分。边缘平台的另一个重要工作是能够：

- 本地分析传入传感器数据
- 快速执行该分析边缘或本地分析是执行对在边缘收集的传感器数据（"本地"）进行评估并基于其看到内容触发驱动或操作的处理。

Why [edge analytics](https://docs.edgexfoundry.org/1.2/general/Definitions/#Edge-Analytics)? Local analytics are important for two reasons:

- 某些决策无法等待传感器收集的数据反馈给企业或云系统并返回响应。
- 此外，某些边缘系统并不总是连接到企业或云 - 它们具有间歇性的连接期。

本地分析允许系统独立运行，至少在某些时间上是这样。例如：当船舶在海上时，运输集装箱的冷却系统必须能够在本地做出决策，而无需长时间使用互联网连接。本地分析还允许系统在系统操作关键时以低潜在方式快速行动。作为一个极端的情况，想象一下，你的车的安全气囊发射的基础上的数据被发送到云和分析碰撞。您的汽车有本地分析，以防止这种潜在的缓慢和容易出错交付在您的汽车的安全驱动。

EdgeX 的构建是为了对从边缘收集的数据进行本地操作。换句话说，事件由本地分析处理，可用于触发传感器/设备上的回工作。

正如应用程序服务准备数据供北端云系统或应用程序使用一样，应用程序服务可以处理和获取 EdgeX 事件（及其包含的传感器数据）到任何分析包（参见步骤 4）。默认情况下，EdgeX 附带一个简单的规则引擎（默认 EdgeX 规则引擎是[Kuiper](https://www.emqx.io/products/kuiper) - EMQ X 的开源规则引擎）。您自己的分析包（或 ML 代理）可以替换或增强本地规则引擎。

![\[外链图片转存失败,源站可能有防盗链机制,建议将图片保存下来直接上传(img-orTFf0Fh-1604370961024)(EdgeXFoundry简介.PIC/EdgeX_step4.png)\]](https://img-blog.csdnimg.cn/20201103103745704.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80Mjg5MjEwMQ==,size_16,color_FFFFFF,t_70#pic_center)


分析包可以探索传感器事件数据，并决定触发设备的驱动。例如，它可以检查发动机的压力读数是否大于 60 PSI。当确定此规则为 true 时，分析包调用核心命令服务以触发某些操作，例如在某些可控制的设备上"打开阀门"（参见步骤 5）。

![\[外链图片转存失败,源站可能有防盗链机制,建议将图片保存下来直接上传(img-El5PNRV5-1604370961027)(EdgeXFoundry简介.PIC/EdgeX_step5.png)\]](https://img-blog.csdnimg.cn/20201103103802879.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80Mjg5MjEwMQ==,size_16,color_FFFFFF,t_70#pic_center)


核心命令服务获取驱动请求，并确定它需要对哪个设备执行该请求;然后调用拥有的设备服务来执行激活（请参阅步骤 6）。Core 命令允许开发人员在激活之前设置其他安全措施或检查。

![\[外链图片转存失败,源站可能有防盗链机制,建议将图片保存下来直接上传(img-tTfAyHVa-1604370961028)(EdgeXFoundry简介.PIC/EdgeX_step6.png)\]](https://img-blog.csdnimg.cn/20201103103818840.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80Mjg5MjEwMQ==,size_16,color_FFFFFF,t_70#pic_center)


设备服务接收激活请求，将其转换为特定于协议的请求，并将请求转发到所需的设备（请参阅步骤 7）。

![\[外链图片转存失败,源站可能有防盗链机制,建议将图片保存下来直接上传(img-zrjnxPbl-1604370961029)(EdgeXFoundry简介.PIC/EdgeX_step7.png)\]](https://img-blog.csdnimg.cn/20201103103833624.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80Mjg5MjEwMQ==,size_16,color_FFFFFF,t_70#pic_center)

# 前提条件
## 需掌握的技术
- docker及docker-compose知道常用命令，了解Dockerfile的格式含义，知道docker-compose.yml格式含义
- Go了解编译运行常用命令，如果跨平台需要自己编译源码
- 熟练使用一门语言C或者Go，方能基于SDK进行设备服务的开发、跨平台开发
- 了解相关主流通讯协议：modbus、mqtt，当然自定义亦可
- 非主要：熟悉git版本管理工具的使用
- 非必须：熟悉cmake、跨平台ide工具
## 必须的软件
### cmake curl安装
```bash
sudo apt update
sudo apt install cmake curl -y
```
### docker安装
[博客](https://blog.csdn.net/weixin_42892101/article/details/109157898)
### IDE VScode的安装
去VScode官网下载对应平台的包安装即可

我这里使用Linux Mint发行版
下载xxx.deb包
```bash
sudo dpkg -i xxx.deb
```
# 部署框架
建个目录EdgeXFoundry，在此执行部署的命令
```bash
mkdir EdgeXFoundry && cd EdgeXFoundry
```
- 在x86平台部署
```bash
curl https://raw.githubusercontent.com/edgexfoundry/developer-scripts/master/releases/geneva/compose-files/docker-compose-geneva-redis-no-secty.yml -o docker-compose.yml; docker-compose up
```
- 在ARM64平台部署
```bash
curl https://raw.githubusercontent.com/edgexfoundry/developer-scripts/master/releases/geneva/compose-files/docker-compose-geneva-redis-no-secty-arm64.yml -o docker-compose.yml; docker-compose up
```
验证是否启动
```bash
docker-compose ps
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20201103110112741.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80Mjg5MjEwMQ==,size_16,color_FFFFFF,t_70#pic_center)
在目录下有yaml的文件，里面官方默认屏蔽了虚拟的随机数设备服务，可以解除屏蔽自己去运行它，官网有相关教程步骤，这里不多做介绍。

**这里给出一份带ui控制台的docker-compose.yml文件**
[UI项目地址](https://github.com/edgexfoundry/edgex-ui-go)
UI使用介绍视频：[优酷](https://v.youku.com/v_show/id_XNDY5NzExNjcyNA==.html)、[youtube](https://www.youtube.com/watch?v=FuR1g64BDE8)
```yaml
# /*******************************************************************************
#  * Copyright 2020 Redis Labs Inc.
#  * Copyright 2020 Intel Corporation.
#  *
#  * Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
#  * in compliance with the License. You may obtain a copy of the License at
#  *
#  * http://www.apache.org/licenses/LICENSE-2.0
#  *
#  * Unless required by applicable law or agreed to in writing, software distributed under the License
#  * is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
#  * or implied. See the License for the specific language governing permissions and limitations under
#  * the License.
#  *
#  * @author: Jim White, Dell
#  * @author: Andre Srinivasan, Redis Labs
#  * @author: Leonard Goodell, Intel
#  * EdgeX Foundry, Geneva, version 1.2.0
#  * added: May 14, 2020
#  *******************************************************************************/

# NOTE:  this Docker Compose file does not contain the security services - namely the API Gateway
# and Secret Store

version: '3.4'

# all common shared environment variables defined here:
x-common-env-variables: &common-variables
  EDGEX_SECURITY_SECRET_STORE: "false"
  Registry_Host: edgex-core-consul
  Clients_CoreData_Host: edgex-core-data
  Clients_Data_Host: edgex-core-data # For device Services
  Clients_Notifications_Host: edgex-support-notifications
  Clients_Metadata_Host: edgex-core-metadata
  Clients_Command_Host: edgex-core-command
  Clients_Scheduler_Host: edgex-support-scheduler
  Clients_RulesEngine_Host: edgex-kuiper
  Clients_VirtualDevice_Host: edgex-device-virtual
  Databases_Primary_Host: edgex-redis
  # Required in case old configuration from previous release used.
  # Change to "true" if re-enabling logging service for remote logging
  Logging_EnableRemote: "false"
  #  Clients_Logging_Host: edgex-support-logging # un-comment if re-enabling logging service for remote logging

volumes:
  db-data:
  log-data:
  consul-config:
  consul-data:

services:
  consul:
    image: edgexfoundry/docker-edgex-consul:1.2.0
    ports:
      - "127.0.0.1:8400:8400"
      - "127.0.0.1:8500:8500"
    container_name: edgex-core-consul
    hostname: edgex-core-consul
    networks:
      - edgex-network
    volumes:
      - consul-config:/consul/config:z
      - consul-data:/consul/data:z
    environment: 
      - EDGEX_DB=redis
      - EDGEX_SECURE=false

  redis:
    image: redis:5.0.8-alpine
    ports:
      - "127.0.0.1:6379:6379"
    container_name: edgex-redis
    hostname: edgex-redis
    networks:
      - edgex-network
    environment:
      <<: *common-variables
    volumes:
      - db-data:/data:z

# The logging service has been deprecated in Geneva release and will be removed in the Hanoi release.
# All services are configure to send logging to STDOUT, i.e. not remote which requires this logging service
# If you still must use remote logging, un-comment the block below, all the related depends that have been commented out
# and the related global override that are commented out at the top.
#
#  logging:
#    image: edgexfoundry/docker-support-logging-go:1.2.1
#    ports:
#      - "127.0.0.1:48061:48061"
#    container_name: edgex-support-logging
#    hostname: edgex-support-logging
#    networks:
#      - edgex-network
#    environment:
#      <<: *common-variables
#      Service_Host: edgex-support-logging
#      Writable_Persistence: file
#      Databases_Primary_Type: file
#      Logging_EnableRemote: "false"
#    depends_on:
#      - consul

  system:
    image: edgexfoundry/docker-sys-mgmt-agent-go:1.2.1
    ports:
      - "127.0.0.1:48090:48090"
    container_name: edgex-sys-mgmt-agent
    hostname: edgex-sys-mgmt-agent
    networks:
      - edgex-network
    environment:
      <<: *common-variables
      Service_Host: edgex-sys-mgmt-agent
      ExecutorPath: /sys-mgmt-executor
      MetricsMechanism: executor
    volumes:
      - /var/run/docker.sock:/var/run/docker.sock:z
    depends_on:
      - consul
#      - logging  # uncomment if re-enabled remote logging
      - scheduler
      - notifications
      - metadata
      - data
      - command

  notifications:
    image: edgexfoundry/docker-support-notifications-go:1.2.1
    ports:
      - "127.0.0.1:48060:48060"
    container_name: edgex-support-notifications
    hostname: edgex-support-notifications
    networks:
      - edgex-network
    environment:
      <<: *common-variables
      Service_Host: edgex-support-notifications
    depends_on:
      - consul
#      - logging  # uncomment if re-enabled remote logging
      - redis

  metadata:
    image: edgexfoundry/docker-core-metadata-go:1.2.1
    ports:
      - "127.0.0.1:48081:48081"
    container_name: edgex-core-metadata
    hostname: edgex-core-metadata
    networks:
      - edgex-network
    environment:
      <<: *common-variables
      Service_Host: edgex-core-metadata
      Service_Timeout: "20000"
      Notifications_Sender: edgex-core-metadata
    depends_on:
      - consul
#      - logging  # uncomment if re-enabled remote logging
      - redis
      - notifications

  data:
    image: edgexfoundry/docker-core-data-go:1.2.1
    ports:
      - "127.0.0.1:48080:48080"
      - "127.0.0.1:5563:5563"
    container_name: edgex-core-data
    hostname: edgex-core-data
    networks:
      - edgex-network
    environment:
      <<: *common-variables
      Service_Host: edgex-core-data
    depends_on:
      - consul
#      - logging  # uncomment if re-enabled remote logging
      - redis
      - metadata

  command:
    image: edgexfoundry/docker-core-command-go:1.2.1
    ports:
      - "127.0.0.1:48082:48082"
    container_name: edgex-core-command
    hostname: edgex-core-command
    networks:
      - edgex-network
    environment:
      <<: *common-variables
      Service_Host: edgex-core-command
    depends_on:
      - consul
#      - logging  # uncomment if re-enabled remote logging
      - redis
      - metadata

  scheduler:
    image: edgexfoundry/docker-support-scheduler-go:1.2.1
    ports:
      - "127.0.0.1:48085:48085"
    container_name: edgex-support-scheduler
    hostname: edgex-support-scheduler
    networks:
      - edgex-network
    environment:
      <<: *common-variables
      Service_Host: edgex-support-scheduler
      IntervalActions_ScrubPushed_Host: edgex-core-data
      IntervalActions_ScrubAged_Host: edgex-core-data
    depends_on:
      - consul
#      - logging  # uncomment if re-enabled remote logging
      - redis

  app-service-rules:
    image: edgexfoundry/docker-app-service-configurable:1.2.0
    ports:
      - "127.0.0.1:48100:48100"
    container_name: edgex-app-service-configurable-rules
    hostname: edgex-app-service-configurable-rules
    networks:
      - edgex-network
    environment:
      <<: *common-variables
      edgex_profile: rules-engine
      Service_Host: edgex-app-service-configurable-rules
      Service_Port: 48100
      MessageBus_SubscribeHost_Host: edgex-core-data
      Binding_PublishTopic: events
    depends_on:
      - consul
#      - logging  # uncomment if re-enabled remote logging
      - data

  rulesengine:
    image: emqx/kuiper:0.4.2-alpine
    ports:
      - "127.0.0.1:48075:48075"
      - "127.0.0.1:20498:20498"
    container_name: edgex-kuiper
    hostname: edgex-kuiper
    networks:
      - edgex-network
    environment:
      # KUIPER_DEBUG: "true"
      KUIPER_CONSOLE_LOG: "true"
      KUIPER_REST_PORT: 48075
      EDGEX_SERVER: edgex-app-service-configurable-rules
      EDGEX_SERVICE_SERVER: http://edgex-core-data:48080
      EDGEX_TOPIC: events
      EDGEX_PROTOCOL: tcp
      EDGEX_PORT: 5566
    depends_on:
      - app-service-rules

  ui:
    image: edgexfoundry/docker-edgex-ui-go:1.2.1
    ports:
      - "127.0.0.1:4000:4000"
    container_name: edgex-ui-go
    hostname: edgex-ui-go
    networks:
      - edgex-network
    environment:
      <<: *common-variables
      Service_Host: edgex-ui-go
    depends_on:
      - consul
#      - logging  # uncomment if re-enabled remote logging
      - scheduler
      - notifications
      - metadata
      - data
      - command

  # Support RulesEngine has been deprecated in the Geneva (1.2.0) release
  # If still required, simply uncomment the block below and comment out the block above.
  #
  # rulesengine:
  #   image: edgexfoundry/docker-support-rulesengine:1.2.1
  #   ports:
  #     - "127.0.0.1:48075:48075"
  #   container_name: edgex-support-rulesengine
  #   hostname: edgex-support-rulesengine
  #   networks:
  #     - edgex-network
  #   depends_on:
  #     - app-service-rules

#################################################################
# Device Services
#################################################################

  device-virtual:
    image: edgexfoundry/docker-device-virtual-go:1.2.2
    ports:
    - "127.0.0.1:49990:49990"
    container_name: edgex-device-virtual
    hostname: edgex-device-virtual
    networks:
      - edgex-network
    environment:
      <<: *common-variables
      Service_Host: edgex-device-virtual
    depends_on:
      - consul
#      - logging  # uncomment if re-enabled remote logging
      - data
      - metadata

  device-rest:
    image: edgexfoundry/docker-device-rest-go:1.1.1
    ports:
      - "127.0.0.1:49986:49986"
    container_name: edgex-device-rest
    hostname: edgex-device-rest
    networks:
      - edgex-network
    environment:
      <<: *common-variables
      Service_Host: edgex-device-rest
    depends_on:
      - data
      - command
  #      - logging  # uncomment if re-enabled remote logging

  # device-random:
  #   image: edgexfoundry/docker-device-random-go:1.2.1
  #   ports:
  #     - "127.0.0.1:49988:49988"
  #   container_name: edgex-device-random
  #   hostname: edgex-device-random
  #   networks:
  #     - edgex-network
  #   environment:
  #     <<: *common-variables
  #     Service_Host: edgex-device-random
  #   depends_on:
  #     - data
  #     - command

#  device-mqtt:
#    image: edgexfoundry/docker-device-mqtt-go:1.2.1
#    ports:
#      - "127.0.0.1:49982:49982"
#    container_name: edgex-device-mqtt
#    hostname: edgex-device-mqtt
#    networks:
#      - edgex-network
#    environment:
#      <<: *common-variables
#      Service_Host: edgex-device-mqtt
#    depends_on:
#      - data
#      - command
#
#  device-modbus:
#    image: edgexfoundry/docker-device-modbus-go:1.2.1
#    ports:
#      - "127.0.0.1:49991:49991"
#    container_name: edgex-device-modbus
#    hostname: edgex-device-modbus
#    networks:
#      - edgex-network
#    environment:
#      <<: *common-variables
#      Service_Host: edgex-device-modbus
#    depends_on:
#      - data
#      - command
#
#  device-snmp:
#    image: edgexfoundry/docker-device-snmp-go:1.2.1
#    ports:
#      - "127.0.0.1:49993:49993"
#    container_name: edgex-device-snmp
#    hostname: edgex-device-snmp
#    networks:
#      - edgex-network
#    environment:
#      <<: *common-variables
#      Service_Host: edgex-device-snmp
#    depends_on:
#      - data
#      - command

networks:
  edgex-network:
    driver: "bridge"
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20201106011042379.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80Mjg5MjEwMQ==,size_16,color_FFFFFF,t_70#pic_center)
Consul访问地址：`http://localhost:8500/ui`
UI控制台访问地址：`http://localhost:4000`
# 设备接入服务开发
登录看了下阿里云的边缘计算，驱动都有市场了，商机无限，相对于EdgeXFoundry的开发，简直傻瓜式部署应用边缘计算框架——一时感慨
![在这里插入图片描述](https://img-blog.csdnimg.cn/20201108000647834.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80Mjg5MjEwMQ==,size_16,color_FFFFFF,t_70#pic_center)

## 安装必须的库文件
```bash
sudo apt-get install libcurl4-openssl-dev libmicrohttpd-dev libyaml-dev libcbor-dev
```
需运行在其他平台的，需要自行编译对应平台的库
看下官网对版本的要求：
* A version of GCC supporting C11.
* CMake version 3 or greater and make.
* Development libraries and headers for:
  * curl (version 7.32 or later)
  * microhttpd (version 0.9)
  * libyaml (version 0.1.6 or later)
  * libcbor (version 0.5)
  * libuuid (from util-linux v2.x)

## 获取官方的SDK
这里使用C
```bash
#指定分支版本
git clone -b 1.2.2 https://github.com/edgexfoundry/device-sdk-c.git
#或者直接下载最新版
git clone https://github.com/edgexfoundry/device-sdk-c.git
```
## 编译SDK
进入源码目录，直接make
```bash
cd device-sdk-c
make
```
顺利无误的情况下将得到x86_64平台的sdk库文件(`release目录下`)，这可能不是我们目标所需平台，先进行测试可用。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20201105152623655.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80Mjg5MjEwMQ==,size_16,color_FFFFFF,t_70#pic_center)
生成的xx.deb不建议使用，开发的设备服务一般情况下是非x86平台，更多可能是ARM64、ARMHF、AARCH64等架构的平台，安装没啥用。
## SDK的使用测试
在源码目录下src/c/example下有模板文件`template.c`
- 按照官方说明，sdk提供了一些接口完善这些接口，即可完成设备接入的开发，与阿里云边缘Link IoT Edge开发类似（设备驱动开发即设备接入）
- 需要完善的接口在`devsdk/devsdk.h`中，当然template.c就是完善后的样版
- 在`src/c/example目录下有较多的例程`，结合xx.yaml与xx.toml文件看对应的源码

以下模板文件可以看出只要完善以下回调接口：
```c
1、完善以下回调接口
    template_init,         /* Initialize */
    template_reconfigure,  /* Reconfigure */
    template_discover,     /* Discovery */
    template_get_handler,  /* Get */
    template_put_handler,  /* Put */
    template_stop          /* Stop */
2、再定义设备提供的资源，如温湿度设备，提供温度、湿度数据信息，提供设备启动关闭、数据采集周期等扩展控制资源
其实这些就类似于阿里云LinkIoTEdge的物模型概念，阿里云的是云端界面配置自动生成json格式，这个就需要自己手写了，而且是yaml格式的，不过提供了不少样版，难度不是太大
3、编译运行
```
```c
/* template implementation of an Edgex device service using C SDK */

/*
 * Copyright (c) 2018-2020
 * IoTech Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#include "devsdk/devsdk.h"
#include "edgex/edgex-base.h"

#include <unistd.h>
#include <signal.h>

#define ERR_CHECK(x) if (x.code) { fprintf (stderr, "Error: %d: %s\n", x.code, x.reason); devsdk_service_free (service); free (impl); return x.code; }

typedef struct template_driver
{
  iot_logger_t * lc;
} template_driver;

static void dump_protocols (iot_logger_t *lc, const devsdk_protocols *prots)
{
  iot_log_debug (lc, " [Other] protocol:");
  for (const devsdk_nvpairs *nv = devsdk_protocols_properties (prots, "Other"); nv; nv = nv->next)
  {
    iot_log_debug (lc, "    %s = %s", nv->name, nv->value);
  }
}

static void dump_attributes (iot_logger_t *lc, const devsdk_nvpairs *attrs)
{
  for (const devsdk_nvpairs *a = attrs; a; a = a->next)
  {
    iot_log_debug (lc, "    %s = %s", a->name, a->value);
  }
}

/* --- Initialize ---- */
/* Initialize performs protocol-specific initialization for the device
 * service.
 */
static bool template_init
(
  void *impl,
  struct iot_logger_t *lc,
  const iot_data_t *config
)
{
  template_driver *driver = (template_driver *) impl;
  iot_log_debug (lc, "Template Init. Driver Config follows:");
  if (config)
  {
    iot_data_map_iter_t iter;
    iot_data_map_iter (config, &iter);
    while (iot_data_map_iter_next (&iter))
    {
      iot_log_debug (lc, "    %s = %s", iot_data_map_iter_string_key (&iter), iot_data_map_iter_string_value (&iter));
    }
  }
  driver->lc = lc;
  iot_log_debug (lc, "Template Init done");
  return true;
}

/* --- Reconfigure ---- */
/* Reconfigure is called if the driver configuration is updated.
 */
static void template_reconfigure
(
  void *impl,
  const iot_data_t *config
)
{
  iot_data_map_iter_t iter;
  template_driver *driver = (template_driver *) impl;
  iot_log_debug (driver->lc, "Template Reconfiguration. New Config follows:");
  iot_data_map_iter (config, &iter);
  while (iot_data_map_iter_next (&iter))
  {
    iot_log_debug (driver->lc, "    %s = %s", iot_data_map_iter_string_key (&iter), iot_data_map_iter_string_value (&iter));
  }
}

/* ---- Discovery ---- */
/* Device services which are capable of device discovery should implement it
 * in this callback. It is called in response to a request on the
 * device service's discovery REST endpoint. New devices should be added using
 * the devsdk_add_device() method
 */
static void template_discover (void *impl) {}

/* ---- Get ---- */
/* Get triggers an asynchronous protocol specific GET operation.
 * The device to query is specified by the protocols. nreadings is
 * the number of values being requested and defines the size of the requests
 * and readings arrays. For each value, the commandrequest holds information
 * as to what is being requested. The implementation of this method should
 * query the device accordingly and write the resulting value into the
 * commandresult.
 *
 * Note - In a commandrequest, the DeviceResource represents a deviceResource
 * which is defined in the device profile.
*/
static bool template_get_handler
(
  void *impl,
  const char *devname,
  const devsdk_protocols *protocols,
  uint32_t nreadings,
  const devsdk_commandrequest *requests,
  devsdk_commandresult *readings,
  const devsdk_nvpairs *qparams,
  iot_data_t **exception
)
{
  template_driver *driver = (template_driver *) impl;

  /* Access the location of the device to be accessed and log it */
  iot_log_debug(driver->lc, "GET on device:");
  dump_protocols (driver->lc, protocols);

  for (uint32_t i = 0; i < nreadings; i++)
  {
    /* Log the attributes for each requested resource */
    iot_log_debug (driver->lc, "  Requested reading %u:", i);
    dump_attributes (driver->lc, requests[i].attributes);
    /* Fill in a result regardless */
    readings[i].value = iot_data_alloc_string ("Template result", IOT_DATA_REF);
  }
  return true;
}

/* ---- Put ---- */
/* Put triggers an asynchronous protocol specific SET operation.
 * The device to set values on is specified by the protocols.
 * nvalues is the number of values to be set and defines the size of the
 * requests and values arrays. For each value, the commandresult holds the
 * value, and the commandrequest holds information as to where it is to be
 * written. The implementation of this method should effect the write to the
 * device.
 *
 * Note - In a commandrequest, the DeviceResource represents a deviceResource
 * which is defined in the device profile.
*/
static bool template_put_handler
(
  void *impl,
  const char *devname,
  const devsdk_protocols *protocols,
  uint32_t nvalues,
  const devsdk_commandrequest *requests,
  const iot_data_t *values[],
  iot_data_t **exception
)
{
  template_driver *driver = (template_driver *) impl;
  /* Access the location of the device to be accessed and log it */
  iot_log_debug (driver->lc, "PUT on device:");
  dump_protocols (driver->lc, protocols);

  for (uint32_t i = 0; i < nvalues; i++)
  {
    /* A Device Service again makes use of the data provided to perform a PUT */
    /* Log the attributes */
    iot_log_debug (driver->lc, "  Requested device write %u:", i);
    dump_attributes (driver->lc, requests[i].attributes);
    switch (edgex_propertytype_data (values[i]))
    {
      case Edgex_String:
        iot_log_debug (driver->lc, "  Value: %s", iot_data_string (values[i]));
        break;
      case Edgex_Uint64:
        iot_log_debug (driver->lc, "  Value: %lu", iot_data_ui64 (values[i]));
        break;
      case Edgex_Bool:
        iot_log_debug (driver->lc, "  Value: %s", iot_data_bool (values[i]) ? "true" : "false");
        break;
      /* etc etc */
      default:
        iot_log_debug (driver->lc, "  Value has unexpected type %s: %s", iot_data_type_name (values[i]), iot_data_to_json (values[i]));
    }
  }
  return true;
}

/* ---- Stop ---- */
/* Stop performs any final actions before the device service is terminated */
static void template_stop (void *impl, bool force) {}

int main (int argc, char *argv[])
{
  sigset_t set;
  int sigret;

  template_driver * impl = malloc (sizeof (template_driver));
  memset (impl, 0, sizeof (template_driver));

  devsdk_error e;
  e.code = 0;

  /* Device Callbacks */
  devsdk_callbacks templateImpls =
  {
    template_init,         /* Initialize */
    template_reconfigure,  /* Reconfigure */
    template_discover,     /* Discovery */
    template_get_handler,  /* Get */
    template_put_handler,  /* Put */
    template_stop          /* Stop */
  };

  /* Initalise a new device service */
  devsdk_service_t *service = devsdk_service_new
    ("device-template", "1.0", impl, templateImpls, &argc, argv, &e);
  ERR_CHECK (e);

  int n = 1;
  while (n < argc)
  {
    if (strcmp (argv[n], "-h") == 0 || strcmp (argv[n], "--help") == 0)
    {
      printf ("Options:\n");
      printf ("  -h, --help\t\t\tShow this text\n");
      devsdk_usage ();
      return 0;
    }
    else
    {
      printf ("%s: Unrecognized option %s\n", argv[0], argv[n]);
      return 0;
    }
  }

  /* Set default config */
  iot_data_t *confparams = iot_data_alloc_map (IOT_DATA_STRING);
  iot_data_string_map_add (confparams, "TestParam1", iot_data_alloc_string ("X", IOT_DATA_REF));
  iot_data_string_map_add (confparams, "TestParam2", iot_data_alloc_string ("Y", IOT_DATA_REF));

  /* Start the device service*/
  devsdk_service_start (service, confparams, &e);
  ERR_CHECK (e);

  /* Wait for interrupt */
  sigemptyset (&set);
  sigaddset (&set, SIGINT);
  sigprocmask (SIG_BLOCK, &set, NULL);
  sigwait (&set, &sigret);
  sigprocmask (SIG_UNBLOCK, &set, NULL);

  /* Stop the device service */
  devsdk_service_stop (service, true, &e);
  ERR_CHECK (e);

  devsdk_service_free (service);
  free (impl);
  iot_data_free (confparams);
  return 0;
}
```
## 测试编译
```bash
export CSDK_DIR=你下载sdk源码目录路径
export CSDK_LIB=你下载sdk编译后生成的动态库文件路径
gcc -I$CSDK_DIR/include -L$CSDK_LIB -o template template.c -lcsdk
```
## 运行
下图只为说明接口获取的方法，设备名在哪看
![在这里插入图片描述](https://img-blog.csdnimg.cn/20201106150348666.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80Mjg5MjEwMQ==,size_16,color_FFFFFF,t_70#pic_center)

```bash
#运行前，请先准备好configuration.toml服务配置文件与xxx.yaml设备配置文件放置在同一个目录下,如dir
./template -c dir

#读取可用API接口
GET http://localhost:48082/api/v1/device/name/configuration.toml文件中的设备名

#获得类似上图中的数据即可
GET 获取的url，例子如下
GET http://localhost:48082/api/v1/device/c5c93b51-226b-4b0f-a4e5-8bfbfa096382/command/2bb9992c-3b74-422a-b4c6-74b33492ca79

PUT 获取的url 需设置的参数，例子如下
PUT http://localhost:48082/api/v1/device/59f51186-af23-4340-823f-0bbd8a65449f/command/db0dbed8-ad94-4706-9f34-6110e2b45797
{"Min_Int8":"-10", "Max_Int8":"10" }

#PUT与GET的数据不尽相同，皆是基于你所定义的设备配置文件中提供的设备资源，而设备配置文件又是基于你依靠SDK编写的设备服务驱动
```
至此，皆是对官网的使用流程进行说明，下面进入正题。
## 使用SDK开发真实设备接入服务
### 着手编写一个温湿度设备接入
#### 准备相关文件及目录
建立一个文件夹如：`temperature-device-driver`
```bash
mkdir temperature-device-driver && cd temperature-device-driver
```
建立`device-temperature.c`文件、建立`res`目录
在`temperature-device-driver`目录下建立`build.sh`脚本文件，内容如下：
```bash
#!/bin/sh

#定义SDK源码头文件目录
SDK_INC_DIR=/home/aron566/Workspace/C_SDK/device-sdk-c/include

#定义SDK动态库文件目录
SDK_LIB_DIR=/home/aron566/Workspace/C_SDK/device-sdk-c/build/release/_CPack_Packages/Linux/TGZ/csdk-1.3.0/lib

#定义编译生成的APP名称
TARGET_APP_NAME=device_driver

#定义源码文件名称
SOURCE_FILE_NAME=device_driver.c

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$SDK_LIB_DIR

case "$1" in
	make)
		gcc -I$SDK_INC_DIR -L$SDK_LIB_DIR -o $TARGET_APP_NAME $SOURCE_FILE_NAME -lcsdk
		;;
	run)
		./$TARGET_APP_NAME -c res
		;;
	*)
		echo "Usage: $0 {make|run}"
		exit 1
esac
```
添加可执行权限
```bash
sudo chmod +x build.sh
```
#### 编写温湿度设备接入设备服务
参考阿里云方式，控制台以每个边缘网关为首向下拓展
- 边缘网关，运行设备服务
 - 控制台中设备服务列表代表着不同功能的边缘网关，实际就是多协议驱动服务
  - 遵照驱动中支持的通讯协议设备，编写不同的设备配置文件（mqtt网关、modbus温度传感器、光照等各类型传感器或者其他设备）
   - 依照现场实际部署使用，在支持的设备驱动的设备服务下建立设备，为其配置特定设备配置文件及其他参数，特别指定设备名（协议-位置信息(应包含上级网关信息+地理位置)-设备名(可重复)-地址号或者序号(同名设备不可重复)）
这样后台浏览设备易于查看：物理通讯TOPO关系、通讯地址及协议、地理区域信息。

*记住，完善接口即可*
```c
#include "devsdk/devsdk.h"
#include "edgex/edgex-base.h"

#include <unistd.h>
#include <signal.h>

#define ERR_CHECK(x) if (x.code) { fprintf (stderr, "Error: %d: %s\n", x.code, x.reason); devsdk_service_free (service); free (impl); return x.code; }


```
#### 配置文件xx.yaml和xx.toml
xx.yaml的修改需遵照设备服务提供的设备功能
[官方对配置的介绍](https://blog.csdn.net/weixin_42892101/article/details/109528246)
[其他配置的介绍](https://blog.csdn.net/keyoushide/article/details/98306622)
#### 编译
```bash
./build make
```
#### 运行
运行可以使用脚本，当然需要在脚本中修改定义好变量
```bash
./build run
```