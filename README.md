# ZUITREE

ZUITREEはCfCAのN体学校・GPU講習会に始まった、C++で書かれたN体重力計算のGPU実装コードです。

## 特徴

- 重力相互作用は直接法とツリー法の2種。PPPT法(Oshino, Funato, & Makino 2011)などは実装するかもしれないししないかもしれない
- GPUによる並列化(予定)
  - CUDAを使うかKokkosを使うか模索中

## ビルドの方法

CMakeを用いてビルドします。以下は基本的な手順です：

```bash
mkdir build
cd build
cmake ..
cmake --build .
ctest
```

`GPU`、`DEBUG`、`OPTIMIZE`といったオプションは`cmake`実行時に指定できます。

```bash
cmake .. -DGPU=managed -DDEBUG=OFF -DOPTIMIZE=ON
```

ドキュメント生成は以下のように行います：

```bash
cmake --build . --target doc
```

## 実行方法

### 手元のCPU環境で実行する場合

上記のビルドが終わった後、build/bin/collapseという実行ファイルが作られます。パラメータをparams.jsonなどのようにして指定し、次のようにして実行してください：(ワーキングディレクトリはbuildとする)
```bash
./bin/collapse ../params.json
```

### CfCA(など)のGPU環境で実行する場合

モジュールのロードなどを含めたシェルスクリプトとしてgpu_batch.shがあります。これをsbatchでGPUクラスタに送ることで実行できます。

## コード開発に関するメモ

特に謎なもの:

- "restrict" キーワードは標準のC++では対応されてないものの、NVHPCではなぜかこれがないときちんと動かない
  - 標準のC++のIntelisenseだとエラーになって見辛くなるので、#ifndefで対応してる
