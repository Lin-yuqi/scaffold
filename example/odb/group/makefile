group:group.cc student-odb.cxx
	g++ -std=c++17 -o $@ $^ -lodb -lodb-mysql -lodb-boost -lpthread
student-odb.cxx: student.h
	odb -d mysql --generate-query --generate-schema --std c++11 --profile boost/date-time $^

clean:
	rm -f group *-odb.* student.sql;