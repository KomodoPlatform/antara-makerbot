# Rest Api Doc

## get price with an base currency and quote currency

Retrieves the median of the price of a base / quote currency pair

**URL** : `/api/v1/getprice?base_currency=:currency&quote_currency=:quote_currency`

**Method** : `GET`

**Auth required** : No

**Permissions required** : None

### Success Response

**Code** : `200 OK`

**Content examples**

```json
{
"price":"0.00007494"
}
```

### Error Response

**Code** : `400 Bad request` (Ill-formed request)

**Code** : `422 Unprocessable Entity` (Wrong Argument)

**Code** : `500 Internal Server Error` (Unexistent Base or Quote, throw pair not available)

## mm2 get orderbook with an base currency and quote currency

Retrieves the order book for the given asset pair

**URL** : `/api/v1/legacy/mm2/getorderbook?base_currency=:currency&quote_currency=:quote_currency`

**Method** : `GET`

**Auth required** : No

**Permissions required** : None

### Success Response

**Code** : `200 OK`

**Content examples**

```json
{
  "askdepth": 0,
  "asks": [
    {
      "address": "RTpwruinq4GLbkGZg8NrJw5YPWUvMWnHYR",
      "age": 10,
      "avevolume": 0,
      "coin": "RICK",
      "depth": 0,
      "maxvolume": 969.8307168,
      "numutxos": 0,
      "price": 2,
      "pubkey": "9727ffa665389a4fed0a2b75687d1a7ad23a430f0aa06dfbf9edb9b972da7808",
      "zcredits": 0
    },
    {
      "address": "RT9MpMyucqXiX8bZLimXBnrrn2ofmdGNKd",
      "age": 3,
      "avevolume": 0,
      "coin": "RICK",
      "depth": 0,
      "maxvolume": 989.7500182,
      "numutxos": 0,
      "price": 2,
      "pubkey": "1bb83b58ec130e28e0a6d5d2acf2eb01b0d3f1670e021d47d31db8a858219da8",
      "zcredits": 0
    }
  ],
  "base": "RICK",
  "biddepth": 0,
  "bids": [
    {
      "address": "RTpwruinq4GLbkGZg8NrJw5YPWUvMWnHYR",
      "age": 10,
      "avevolume": 0,
      "coin": "MORTY",
      "depth": 0,
      "maxvolume": 963.1034758799999,
      "numutxos": 0,
      "price": 0.5,
      "pubkey": "9727ffa665389a4fed0a2b75687d1a7ad23a430f0aa06dfbf9edb9b972da7808",
      "zcredits": 0
    },
    {
      "address": "RT9MpMyucqXiX8bZLimXBnrrn2ofmdGNKd",
      "age": 4,
      "avevolume": 0,
      "coin": "MORTY",
      "depth": 0,
      "maxvolume": 973.91801759,
      "numutxos": 0,
      "price": 0.5,
      "pubkey": "1bb83b58ec130e28e0a6d5d2acf2eb01b0d3f1670e021d47d31db8a858219da8",
      "zcredits": 0
    }
  ],
  "netid": 0,
  "numasks": 2,
  "numbids": 2,
  "rel": "MORTY",
  "timestamp": 1565975342
}
```

### Error Response

**Code** : `400 Bad request` (Ill-formed request)

**Code** : `422 Unprocessable Entity` (Wrong Argument)

**Code** : `500 Internal Server Error` (Unexistent Base or Quote, Or `mm2` return 500 error code + message)