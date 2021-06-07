ifneq (,$(findstring xterm,${TERM}))
	BLACK        := $(shell tput -Txterm setaf 0)
	RED          := $(shell tput -Txterm setaf 1)
	GREEN        := $(shell tput -Txterm setaf 2)
	YELLOW       := $(shell tput -Txterm setaf 3)
	LIGHTPURPLE  := $(shell tput -Txterm setaf 4)
	PURPLE       := $(shell tput -Txterm setaf 5)
	BLUE         := $(shell tput -Txterm setaf 6)
	WHITE        := $(shell tput -Txterm setaf 7)
	RESET 		 := $(shell tput -Txterm sgr0)
else
	BLACK        := ""
	RED          := ""
	GREEN        := ""
	YELLOW       := ""
	LIGHTPURPLE  := ""
	PURPLE       := ""
	BLUE         := ""
	WHITE        := ""
	RESET        := ""
endif

all: tests clean run


test-rapid-json: test/test-rapid-json.cpp 
	@echo ${WHITE}Testing ${PURPLE}Rapid JSON ${WHITE}library 
	@g++ test/test-rapid-json.cpp -o test-rapid-json

test-curl: test/curl.cpp
	@echo ${WHITE}Testing ${PURPLE}curl ${WHITE}dependency
	@g++ test/curl.cpp -o test-curl -lcurl

test-riot-api: test/riot-api-test.cpp
	@echo ${WHITE}Testing ${PURPLE}Riot API
	@g++ test/riot-api-test.cpp classes/Curl/UrlReader.cpp -o test-riot-api -lcurl

tests: test-curl test-rapid-json test-riot-api
	@./test-curl > output.txt 
	@diff output.txt test/curl-test-output.txt
	@./test-rapid-json > output.txt 
	@diff output.txt test/rapid-json-test-output.txt
	@./test-riot-api > output.txt
	@diff output.txt test/riot-api-test-output.txt
	@echo "${GREEN}Tests Passed Successfully${RESET}"

	
clean:
	@rm -f test-curl test-rapid-json test-riot-api output.txt
	@echo "${PURPLE}Cleaned Files${RESET}"

install:
	@echo "${GREEN}Compiling Program...${RESET}"
	@g++ -o program.sh -lncurses -lcurl main-program.cpp  classes/UI/Selectable.cpp classes/Curl/UrlReader.cpp classes/Managers/InputManager.cpp classes/Managers/DisplayManager.cpp classes/Riot/MatchPlayer.cpp classes/Riot/Match.cpp  classes/Riot/Utility.cpp classes/Managers/MatchStorage.cpp classes/UI/Buttons/Buttons.cpp classes/UI/Labels/Labels.cpp classes/UI/Lists/Lists.cpp classes/UI/Boxed.cpp classes/UI/Element.cpp classes/UI/InputBox.cpp classes/UI/Screen.cpp classes/Managers/Controller.cpp 

run: install
	@./program.sh
	