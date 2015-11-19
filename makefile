all: twitterTrend

twitterTrend: twitterTrend.c 
	gcc -pthread -o twitterTrend twitterTrend.c

clean:
	rm twitterTrend *.result
