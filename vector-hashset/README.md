
assignment 3 of progmramming paradigms


## compilation
```sh
make
```

## data files for testing
```sh
rm -rf data/
make data
```

## testing
```sh
./vector-test
./hashset-test
```

```sh
./vector-test | diff sample-output-vector.txt -
./hashset-test | diff sample-output-hashset.txt -
```

