server:
	clear
	python server.py 10000 -n 8 -m 8 -NC 2 -TL 90 -LOG server.log
old:
	clear
	python client.py 0.0.0.0 10000 old.sh
new:
	clear
	python client.py 0.0.0.0 10000 new.sh -mode GUI
clean:
	find . -name '*.pyc' -delete

