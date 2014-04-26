
change_proj()
{
	PWD=`pwd`

	echo ".................... Cleaning old project ...................."

	if ! [ -f ./.project_root ] || [ "`cat ./.project_root`" != "$PWD" ] ; then
		rm -rf build_dir/
		rm -rf staging_dir/
		rm -rf tmp/
	else
		if [ -n $DBG_LEVEL ]; then
			make clean $DBG_LEVEL
		else
			make clean
		fi
		rm -rf build_dir/linux*
		rm -rf build_dir/mips*
		rm -rf build_dir/target*
		rm -rf staging_dir/target*
		rm -rf tmp/
	fi

	echo ".................... Preparing new project ...................."

	if [ -e "./projects/common" ] ; then
		cp -rf ./projects/common/. .
	fi

	if [ -e "./projects/$1" ] ; then
		cp -rf ./projects/$1/. .
	fi

	echo `date` $1 >> "./.project_history"

	if [ -n $DBG_LEVEL ]; then
		make prepare $DBG_LEVEL
		make kernel_oldconfig $DBG_LEVEL
	else
		make prepare
		make kernel_oldconfig >& /dev/null
	fi

	echo -n "$PWD" > "./.project_root"
}

if [ -n "$1" ] ; then
	if [ -e "./projects/$1" ] ; then
		change_proj $1
	else
		echo "syntax: $1 [project]"
		echo "    [project] - project name"
	fi
else
	while true ; do
		USER_CHOICE=0
		# prompt
		echo
		echo "9) 904 LTE"
		echo
		echo "0) exit"
		echo
		read -p "Please input your choice -> " USER_CHOICE
		# check choice
		case ${USER_CHOICE} in
		  0)
				exit
				;;
		  9)
				change_proj 904lte
				exit
				;;
		  *)
				echo "bad choice!"
				;;
		esac
	done
fi

