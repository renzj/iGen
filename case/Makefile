CPPFLAGS = -g -std=c++11 -lpthread -lglog -lboost_system -lboost_thread -ldl
INC = -I../core -I../common -I../generator -I/usr/local/include/glog -L/usr/lib/x86_64-linux-gnu -I/usr/local/include -L/usr/local/lib -I../ThreadPool -I../fitting

objects = ../common/parameter.o ../generator/normal_generator.o ../common/closed_system_parameter.o ../generator/random.o ../generator/constant_generator.o ../generator/lognormal_generator.o ../core/helper.o ../core/operator_adapter.o ../core/closed_system.o ../common/record.o ../common/metric.o ../generator/timer_generator.o ../core/open_system.o ../common/open_system_parameter.o ../generator/exponent_generator.o ../fitting/lagrange_fitting.o ./demo.o ../fitting/linear_fitting.o ../fitting/exponential_fitting.o ../fitting/logarithmic_fitting.o ../fitting/polynomial_fitting.o ../generator/pareto_generator.o

demo.so: $(objects)
	g++ -o demo.so $(objects) $(INC) $(CPPFLAGS) -fPIC -shared

$(objects): %.o:%.cpp
	g++ -c $(INC) $? -o $@ -fPIC $(CPPFLAGS)

.PHONY: clean
clean:
	rm demo.so $(objects)
