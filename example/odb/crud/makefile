crud:crud.cc student-odb.cxx
	g++ -std=c++17 -o $@ $^ -lodb -lodb-mysql -lodb-boost -lpthread

student-odb.cxx:student.h
	odb -d mysql --std c++11 --generate-query --generate-schema --profile boost/date-time $^

clean:
	rm -rf *-odb.*  *.sql crud