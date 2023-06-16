examples:
	go build
	./fbeproj

test:
	go test -v -count=1 -cover ./...| grep -v "^?"

bench:
	go test -v -bench=. -run=^# ./...| grep -v "^?"

.PHONY: test examples bench
