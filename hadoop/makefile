all: compile jar

compile: DegreeCount.java FrequencyCount.java DegreeCountRmat.java
	rm -rf degreecount_classes
	rm -rf frequencycount_classes
	rm -rf degreecountrmat_classes
	mkdir degreecount_classes
	mkdir frequencycount_classes
	mkdir degreecountrmat_classes
	javac -classpath ${HADOOP_PREFIX}/hadoop-core-1.2.1.jar -d degreecount_classes/ DegreeCount.java
	javac -classpath ${HADOOP_PREFIX}/hadoop-core-1.2.1.jar -d frequencycount_classes/ FrequencyCount.java
	javac -classpath ${HADOOP_PREFIX}/hadoop-core-1.2.1.jar -d degreecountrmat_classes/ DegreeCountRmat.java

jar: $(compile)
	jar -cvf dist/degreecount.jar -C degreecount_classes .
	jar -cvf dist/frequencycount.jar -C frequencycount_classes .
	jar -cvf dist/degreecountrmat.jar -C degreecountrmat_classes .

rem_deg:
	${HADOOP_PREFIX}/bin/hadoop dfs -rmr output_deg

run_deg: $(jar)
	${HADOOP_PREFIX}/bin/hadoop jar degreecount.jar graph.mining.DegreeCount /user/smransho/graphs/facebook.graph /user/smransho/output_deg/

rem_freq:
	${HADOOP_PREFIX}/bin/hadoop dfs -rmr output_freq

run_freq: $(jar)
	${HADOOP_PREFIX}/bin/hadoop jar frequencycount.jar graph.mining.FrequencyCount /user/smransho/output_deg/part-r-00000 /user/smransho/output_freq/
