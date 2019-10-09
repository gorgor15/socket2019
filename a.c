
while(true)
do
monthArray=(31 28 31 30 31 30 31 31 30 31 30 31)
echo -e "1.해당달출력, 2. 해당 연도 전체 출력"
read a
	if [ $a -eq 2 ]
	then
		
		echo "Input year"
		read year
		echo "Input month"
		read month
	
		firstyear=1900
		mon=1
		allday=1
		p=1
		tmp=1
			echo -e "\t\t$year년 $month월"
			echo -e "일\t월\t화\t수\t목\t금\t토"
		while [ $firstyear -lt $year ]
		do
			if [ `expr $firstyear % 4` -eq 0 ]
			then
				if [ `expr $firstyear % 100` -eq 0 ]
				then
					if [ `expr $firstyear % 400` -eq 0 ]
					then
						yun=1
					else
						yun=0
					fi
				else				
				yun=1
				fi
			
			if [ $yun -eq 1 ]
			then
				yday=366
			else
				yday=365
			fi
		else
			yun=0
		fi
			allday=`expr $allday + $yday`
			firstyear=`expr $firstyear + 1`
		done

			while [ $mon -lt $month ]
			do
			if [ `expr $year % 4` -eq 0 ]
			then
				if [ `expr $year % 100` -eq 0 ]
				then
					if [ `expr $year % 400` -eq 0 ]
					then
						yun=1
					else
						yun=0
					fi
				else				
				yun=1
				fi
			else
				yun=0
			fi
		mday=${monthArray[`expr $mon - 1`]}
		if [ $mon -eq 2 ] && [ $yun -eq 1 ]
		then
			mday=`expr $mday + 1`
		fi
		allday=`expr $allday + $mday`
		mon=`expr $mon + 1`
		done

		week=`expr $allday % 7`
		while [ $p -le $week ]
		do
			echo -en "\t"
			p=`expr $p + 1`
		done
		p=0
		day=${monthArray[`expr $month - 1`]}
		if [ $month -eq 2 ] 
		then
			if [ $yun -eq 1 ]
			then
			day=`expr $day + 1`
			fi
		fi
		
		while [ $tmp -le $day ]
		do
			week=`expr $week + 1`
			if [ $tmp -lt 10 ]
			then
				echo -ne "$tmp \t"
				if [ $week -ge 7 ]
				then
					echo
					week=0
				fi
			else
				echo -ne "$tmp \t"
				if [ $week -ge 7 ]
				then
					echo
					week=0
				fi
			fi
		
		tmp=`expr $tmp + 1`
		done
		echo
		fi
done
