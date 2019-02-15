# lp-cli
Cli client for local-paste written in c.

## Requirements.

To build `lpcli` you will need:

* libcurl

## Building lpcli

```bash
$ git clone https://github.com/girish946/lp-cli
$ cd lp-cli/src
$ make
```

## Using lpcli

```bash
# To get all available pastes.
$ ./lpcli GET -i all
# To get a particular paste
$ ./lpcli GET -i <paste-id>
```
