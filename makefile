.SUFFIXES:
.SECONDARY:   ## prevent all intermediate files to be deleted

srcs        := $(wildcard *.cpp)
headers     := $(wildcard *.hpp)
objects     := $(addsuffix .o, $(basename $(srcs)))
exefile     := main_exe
comp        := clang++
comp_flags  := -std=c++11 -Wfatal-errors
converter   := rsvg-convert -a
pngs_width  := 1024

all_basenames := fig1 fig2 fig3 fig4 fig5 fig6
all_svgs      := $(addsuffix .svg, $(all_basenames))


uname:=$(shell uname -s)

ifeq ($(uname),Darwin)
   pdfviewer := open
   pngviewer := open
   svgviewer := open
else
   pdfviewer  := evince
   pngviewer  := eog
   svgviewer := eog
endif

x%: fig%.pdf
	$(pdfviewer) $< &

y%: fig%.png
	$(pngviewer) $< &

z%: fig%.svg
	$(svgviewer) $< &

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
	rm -f $(objects) $(exefile) *_exe fig*.pdf fig*.svg fig*.png
