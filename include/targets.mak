%.h.gch: %.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -x c++-header -o $@ -c $<

%.cpp.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ -c $<
