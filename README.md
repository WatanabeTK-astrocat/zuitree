# ZUITREE

ZUITREEは国立天文台 天文シミュレーションプロジェクト(CfCA)の[N体学校](https://www.cfca.nao.ac.jp/content/2024%E5%B9%B4%E5%BA%A6-n%E4%BD%93%E3%82%B7%E3%83%9F%E3%83%A5%E3%83%AC%E3%83%BC%E3%82%B7%E3%83%A7%E3%83%B3%E7%AB%8B%E6%98%A5%E3%81%AE%E5%AD%A6%E6%A0%A1)・[GPU講習会](https://www.cfca.nao.ac.jp/content/gpuws2025)に始まった、C++で書かれたN体重力計算のGPU実装コードです。

開発者: [渡邊 太一](https://watanabetk-astrocat.github.io/) (総研大天文/国立天文台)

## 特徴

- 重力相互作用は直接法とOct-treeによるツリー法(Barnes & Hut 1986)の2種。PPPT法(Oshino, Funato, & Makino 2011)などは実装するかもしれないししないかもしれない
- GPUによる並列化(予定)
  - 現時点ではOpenACCを用いて直接法に対するGPU化が完了
  - 指示文方式ではツリー法の並列化は難しいため、CUDAを使うかKokkosを使うか模索中
- Doxygenによるドキュメント
- Git/GitHubを使った管理を頑張る (あくまで頑張る)

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

Doxygenを用いたドキュメント生成は以下のように行います：

```bash
cmake --build . --target doc
```

## 実行方法

### 手元のCPU環境で実行する場合

上記のビルドが終わった後、build/bin/collapseという実行ファイルが作られます。JSON形式で書かれたシミュレーションパラメータをparams.jsonなどのようにして指定し、次のようにして実行してください：(ワーキングディレクトリはbuildとする)

```bash
./bin/collapse ../params.json
```

### CfCAなどのGPU環境で実行する場合

モジュールのロードなどを含めたシェルスクリプトとしてgpu_batch.shがあります。これをsbatchでGPUクラスタに送ることで実行できます。

### シミュレーションパラメータについて

- ```animation_bool```: Gnuplotを使用したアニメーションを行うかどうかのフラグ。デフォルト: false
- ```tree_bool```: ツリー法を使うかどうかのフラグ。デフォルト: false
- ```n```: 粒子数。デフォルト: 1024
- ```r_v```: 初期ビリアル比。デフォルト: 0.2
- ```eps```: ソフトニングパラメータ。デフォルト: 0.015625
- ```theta```: ツリー法を使用する場合の$\theta_c$パラメータ。デフォルト: 0.1
- ```dt```: 固定の時間刻み。デフォルト: 0.0078125
- ```T_end```: シミュレーション時間。デフォルト: 10.0
- ```T_out```: スナップショットの出力時間刻み。デフォルト: 0.125

## コード開発に関するメモ

### 特に謎なもの

- "restrict" キーワードは標準のC++では対応されてないものの、NVHPCではなぜかこれがないときちんと動かない
  - 標準のC++のIntelisenseだとエラーになって見辛くなるので、#ifndefで対応してる

### TODO

- ツリー法のNODEを配列を用いて書き換える
- ツリー法の部分をCUDAを使って書き換える
- ランダム球以外の初期状態を実装する
- Gnuplot以外でもアニメーションできるようにスナップショットを出力する
- ドキュメントに英語を加える
