#!/bin/bash

./bin/dex2oatmd --runtime-arg -Xms64m --runtime-arg -Xmx64m --runtime-arg -classpath --runtime-arg /home/merck/Projects/dtools/art/Loop.jar --runtime-arg -compiler-filter:speed --host --boot-image=/home/merck/Projects/mydroid/out/host/linux-x86/framework/core.art --dex-file=/home/merck/Projects/dtools/art/Loop.jar --oat-file=/tmp/android-data/dalvik-cache/home@merck@Projects@dtools@art@Loop.jar@classes.dexm --oat-location=/tmp/android-data/dalvik-cache/home@merck@Projects@dtools@art@Loop.jar@classes.dexm --android-root=/home/merck/Projects/mydroid
