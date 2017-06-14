.SUFFIXES:
.PHONY: clean, allpdfs, allsvgs, allpngs, x1,x2,x3,x4,x5,x6, y1,y2,y3,y4,y5,y6, z1,z2,z3,z4,z5,z6


srcs        := $(wildcard *.cpp)
headers     := $(wildcard *.hpp)
objects     := $(addsuffix .o, $(basename $(srcs)))
exefile     := main
comp        := clang++
comp_flags  := -std=c++11 -Wfatal-errors
converter   := rsvg-convert -a
pngs_width  := 256

uname:=$(shell uname -s)

ifeq ($(uname),Darwin)
	pdfviewer := open
else
	pdfviewer := evince
endif

x%: fig%.pdf
	$(pdfviewer) $<

y%: fig%.png
	open $<

z%: fig%.svg
	open $<

allsvgs: fig1.svg fig2.svg fig3.svg fig4.svg fig5.svg fig6.svg

allpdfs: fig1.pdf fig2.pdf fig3.pdf fig4.pdf fig5.pdf fig6.pdf

allpngs: fig1.png fig2.png fig3.png fig4.png fig5.png fig6.png

fig%.svg: $(exefile)
	./$(exefile) $* $@

$(exefile): $(objects) makefile
	$(comp)  -o $(exefile) $(objects)

%.o : %.cpp $(headers)
	$(comp) $(comp_flags) -c -o $@ $<

fig%.pdf : fig%.svg
	$(converter)  -o $@ -f pdf $<

fig%.png : fig%.svg
	$(converter) -w $(pngs_width) -o $@ -f png $<


clean:
	rm -f $(objects) $(exefile) fig*.pdf fig*.svg fig*.png
