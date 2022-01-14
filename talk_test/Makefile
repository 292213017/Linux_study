all: talkA talkB

talkA:talkA.cpp
	g++ $< -o $@

talkB:talkB.cpp
	g++ $< -o $@

.PHONY :clean
clean:
	rm -rf talkA talkB