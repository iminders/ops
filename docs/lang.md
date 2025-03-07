```
$b1_v / ($b1_v + $a1_v);

ts_std($price, 60);

ts_std($price, 120);

$price / ts_max($price, 60);

$price / ts_min($price, 60);

$price / ts_ref($price, 120) - 1.0;

$price / ts_ema($price, 60);

```