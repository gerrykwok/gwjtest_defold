#!/bin/sh

# 根据launch_origin.jpg生成ios所用的启动图

allsize='
320 480
640 960
640 1136
750 1334
1334 750
768 1024
1536 2048
1024 768
1242 2208
2208 1242
2048 1536
2048 2732
2732 2048
1668 2224
2224 1668
1125 2436
2436 1125
'

# /sh/create_sub_image.exe -in weixin_20180517190820.jpg -out out.png -scaleX 16 -scaleY 9

# 函数: 获取一个字符串数组的第i个字符串(i从0开始)
func_get_i_string()
{
	str=$1
	i=$2
	j=0
	for s in $str; do
		if [ "$i" == "$j" ]; then
			echo $s
			break;
		fi
		j=$((j+1))
	done
}

func_format_decimal()
{
	dec=$1
	while [ "${dec:(-1)}" == "0" ]; do
		len=${#dec}
		dec=${dec:0:len-1}
	done
	if [ "${dec:0:1}" == "." ]; then
		dec="0"$dec
	fi
	echo $dec
}

func_get_scale()
{
	a=$1
	b=$2
	scale=$(echo "scale=50;$a/$b" | bc)
	scale=$(func_format_decimal $scale)
	echo $scale
}

j=0
w=0
h=0
for i in $allsize; do
	if [ "$j" == "0" ]; then
		w=$i
		j=1
	else
		h=$i
		j=0

		# 生成图片
		rm -f clip.png 2> /dev/null
		sleep 1
		/sh/create_sub_image.exe -in launch_origin.jpg -out clip.png -scaleX $w -scaleY $h
		sleep 1
		pnginfo=$(/sh/getpnginfo -s clip.png)
		clipW=$(func_get_i_string "$pnginfo" 0)
		clipH=$(func_get_i_string "$pnginfo" 1)
		scale=$(func_get_scale $w $clipW)
		echo targetsize=${w}x${h} clip=${clipW}x${clipH} scale=$scale

		/sh/scaleimage.sh -s clip.png -d launch_${w}x${h}.png -scale $scale
		sleep 1
	fi
done
      
rm -f clip.png 2> /dev/null
