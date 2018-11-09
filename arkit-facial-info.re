#@# VRコンテンツのつくりかた3 技術書典4
#@# （）かっこは全角統一する。
#@# ユーザ入力は""ダブルクォーテーション、メニュー表示文字列は「」かぎかっこにでくくる。

= ARKit Facial Trackingについて

//image[arkit-01][iOS ARKit][scale=1.0]

ARKit Facial Trackingとは、ARKitとiPhone Xを用いたフェイストラッキングする機能です。
iPhone Xに搭載されたTrueDepthカメラを用いて顔を（カメラ画像のみを用いる類似の技術と比較して）とても正確にトラッキングします。

また、顔内の各パーツの位置を50種類以上のパラメータとして取得することができます。
目や口の位置と開き具合といったよくあるものから、唇を突き出す@<code>{mouthFunnel}や@<code>{mouthPucker}ではキス顔が取れますし、iOS 12.0からサポートとなった@<code>{tongueOut}ではアカンベーが取れます。
個人差はあるものの、取得できる値はとても正確で扱いやすく、かんたんに2Dや3Dキャラクターに適用することができます。

Apple公式サイトのビデオ@<fn>{arkit-apple}が簡潔でわかりやすく、こちらを一度見ておくことをオススメします。
親切にテキスト起こしもされています。

//footnote[arkit-apple][Face Tracking with ARKit @<href>{https://developer.apple.com/videos/play/tech-talks/601/}]

== Facial Trackingの詳細

空間上の顔の位置と向きを正確に取得します。
位置については上下左右の他、奥行きもそれなりに取得します。
向きについても、顔のパーツが隠れないRoll方向はもちろん、YawやPitch方向もかなりがんばって追従します。

また、50種以上の顔のパーツの位置について、0.0から1.0までその強さを数値で表されたBlendShapeのDictionaryとして取得できます。
以下に取得可能なBlendShape箇所の一覧@<fn>{arkit-blendshape-location}を示します。

//table[arkit-blendshapes][BlendShape Location一覧]{
eyeBlinkLeft	eyeLookDownLeft	eyeLookInLeft	eyeLookOutLeft
eyeLookUpLeft	eyeSquintLeft	eyeWideLeft	eyeBlinkRight
eyeLookDownRight	eyeLookInRight	eyeLookOutRight	eyeLookUpRight
eyeSquintRight	eyeWideRight
jawForward	jawLeft	jawRight	jawOpen
mouthClose	mouthFunnel	mouthPucker	mouthLeft
mouthRight	mouthSmileLeft	mouthSmileRight	mouthFrownLeft
mouthFrownRight	mouthDimpleLeft	mouthDimpleRight	mouthStretchLeft
mouthStretchRight	mouthRollLower	mouthRollUpper	mouthShrugLower
mouthShrugUpper	mouthPressLeft	mouthPressRight	mouthLowerDownLeft
mouthLowerDownRight	mouthUpperUpLeft	mouthUpperUpRight
browDownLeft	browDownRight	browInnerUp	browOuterUpLeft
browOuterUpRight	cheekPuff	cheekSquintLeft	cheekSquintRight
noseSneerLeft	noseSneerRight	tongueOut
//}

//footnote[arkit-blendshape-location][ARFaceAnchor.BlendShapeLocation @<href>{https://developer.apple.com/documentation/arkit/arfaceanchor/blendshapelocation}]

具体的に@<code>{jawOpen}について見てみましょう。

//image[arkit-jawopen][BlendShapeLocation jawOpen][scale=0.70]

一番あごを閉じた状態で0.0、一番開いた状態で1.0が取得できます。
このように、全てのBlendShapeについて白マスクの画像が載っていますのでドキュメントのみでどのパラメータが顔のどの箇所を担当しているのか、分かるようになっています。

//footnote[arkit-jawopen][jawOpen @<href>{https://developer.apple.com/documentation/arkit/arfaceanchor/blendshapelocation/2928236-jawopen}]

== ARKitとしてのトラッキングとモード

ARKitの一機能であるため、「iPhoneをバーチャル空間を覗き込む窓として」カメラが捉えた顔のトラッキング結果を返す動きをします。
これが少しややこしいので、気をつけましょう。

@<code>{ARConfiguration.WorldAlignment}@<fn>{arkit-worldalignment}設定によって、現実世界の情報にどれだけ追従して3軸を返すか、という違いがあります。

 * @<code>{gravity}：鉛直方向をY軸として、ディスプレイ正面の向きをZ軸、同じくディスプレイの左右の向きをX軸とするモード
 * @<code>{gravityAndHeading}：鉛直方向をY軸として、南北をZ軸、東西をX軸とするモード
 * @<code>{camera}：重力や地磁気方向を考慮せず、ディスプレイ正面の向きをZ軸、左右の向きをX軸、上下の向きをY軸とするモード

現実世界を拡張するAR的な使い方をしたい際には@<code>{gravity}や@<code>{gravityAndHeading}を、そうではない際には@<code>{camera}のモードを設定しましょう。

//footnote[arkit-worldalignment][@<href>{https://developer.apple.com/documentation/arkit/arsessionconfiguration/worldalignment}]

== TrueDepthカメラと顔の位置検出

TrueDepthカメラを用いていると言われていますが、点滅している（携帯のカメラプレビューなどで確認できます）赤外線ライト部を隠してもトラッキングが継続できたりと謎が多く、詳細な動作については明かされていません。

トラッキングを動かしつつ、XcodeでCPU負荷を観測していると、「顔の位置をトラッキングするモード」と「顔の位置がトラッキングできた上で、顔の各パーツのトラッキングするモード」の2段階を切り替えながら動作しているように見えます。

また、前述の公式ビデオでは1秒間に60回トラッキングしているとのことですが、実際にコールバックを数えてみると、概ね秒間50回のようです。
