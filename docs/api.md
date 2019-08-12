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