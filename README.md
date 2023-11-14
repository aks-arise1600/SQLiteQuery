# SQLiteQuery
Qt based SQLite configure and queries

## Install dependencies

	sudo apt update
	sudo apt install -y build-essential qt5-qmake qtbase5-dev qtbase5-dev-tools cmake git
	
## Verify dependencies

		qmake --version
		cmake --version
		gcc --version	

## Cmake build

		git clone https://github.com/anil-arise1600/SQLiteQuery.git
		cd SQLiteQuery
		mkdir cBuild && cd cBuild
		cmake ..
		make
		
		
## Qmake build

		git clone https://github.com/anil-arise1600/SQLiteQuery.git
		cd SQLiteQuery
		mkdir qBuild && cd qBuild
		qmake ..
		make
		
		
		
## Run example 

		./sqlite_example/sqlite_example
		
## Close example
	
	Press Ctrl + C
	
	
		

