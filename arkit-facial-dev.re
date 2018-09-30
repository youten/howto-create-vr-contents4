#@# VRコンテンツのつくりかた3 技術書典4
#@# （）かっこは全角統一する。
#@# ユーザ入力は""ダブルクォーテーション、メニュー表示文字列は「」かぎかっこにでくくる。

= UnityでARKit Facial Trackingで遊ぶ

== slothアライグマサンプル

Unity ARKit Pluginに含まれるsloth、アライグマのサンプルが一番目的に近いので、まずそちらを動かします。

Unityのバージョンですが、実は少しややこしい話があります。
後述のARKitRemoteというデバッグ用アプリが新しめのUnityだと動かないため、LTSの2017.4系最新が無難そうです。
本書では2017.4.7f1という（中途半端に）古めのUnityで動作確認をしています。

ARKit Face Trackingそのものは2017.4系〜2018.2系でチマチマ動かしていますが設定等や使い勝手に特に差はない認識です。
.NET 4.x対応の新しいC#で書きたい人は、ARKitRemoteだけ古いUnityでビルドして、アプリの開発は2018.2系を使うのをオススメします。

=== プロジェクト作成とUnity ARKit Plugin

 * iPhone XとOSバージョンに対応するXcodeがインストールされたMacでUnityを起動します。
 * 新規にUnity 3Dプロジェクトを作成します。
 * 「File＞Build Settings...」からiOSに「Switch Platform」します。
 * Asset Storeで「Unity ARKit Plugin」をダウンロードし、importします。
 * 「UnityARKitPlugin＞Resources＞UnityARKitPlugin＞ARKitSettings」を選択し、「AR Kit Uses Facetracking」と「App Requires AR Kit」のチェックをONにします。
 * 「Edit＞Project Settings＞Player」から、「Other Settings」を開き、「Target minimum iOS Version」をARKit Face Trackingにあわせて11.0にします。
 * 「UnityARKitPlugin/Examples/FaceTracking/FaceBlendShapeSloth」を開きます。
 * 再度「File＞Build Settings...」から「Add Open Scenes」を選択し、FaceBlendShapeシーンを追加後、「Build And Run」を選択します。
 * 初回ビルド時にはXcodeプロジェクトの生成先のパスを聞く、フォルダ選択ダイアログが出てきます。"Xcode"等、適当なフォルダ名を入力しましょう。

Unityでビルド成功後、Xcodeで引き継いだビルドがうまく行けばiPhone X上で自分の顔をトラッキングするアライグマが表示されます。
初回はXcode上でSigningエラーが出ます。有効なApple IDをXcodeに設定しておけば、無料でPersonal Teamの署名ができるはずなので、そのあたりは適当に調べてみてください。

また、Unity ARKit PluginはBitBucketでOSSとして公開されています。
ARKitの最新の機能を使う際にはこちらを使う必要があるタイミングもありますが、本件においてはAsset Storeからダウンロードできるバージョンで問題ありません。

https://bitbucket.org/Unity-Technologies/unity-arkit-plugin

Unity ARKit Pluginは全体としてはMITライセンスで、SlothCharacter配下のみUCL（Unity Companion License）ライセンスと記載されています。
後者はざっくりで言うと「Unityとして組み込んで際に限っては特別の制限はないよ」というライセンスに読めます。
Unity ARKit Plugin配下の通常ロジックのソースコードはMITということで、サンプルに付属するコードの改造・再配布は問題がなさそうです。

=== Unityエディタ上でリモートデバッグができるARKitRemote

iPhone X上でデバッグ専用のアプリを動かして、Unityエディタ上でトラッキング結果を受け取るARKitRemoteという仕組みがありますので、まずは準備します。

ところがこのARKitRemote、Unity2018.2あたりからNativeまわりの動きが変わったのか、少し古めのUnityでビルドする必要があります。

 * 「File＞Build Settings...」から「UnityARKitPlugin/ARKitRemote/ARKitRemote」のみチェックをONにします。
 * 「Development Build」のチェックをONにします。
 * 別アプリとしてビルドするために、「PlayerSettings」の「Product Name」を"ARKitRemote"に、「Other Settings」の「Bundle Identifier」を"com.unity.arkitremote"あたりの別名に変えておきます。
 * 「File＞Build and Run」を選択し、Xcodeビルドまで成功後、起動するのを待つ。今後、動きが怪しいことになった際にはARKitRemoteアプリのタスクキル＋再起動が一番復帰率が高いことを覚えておく。
 * Unityエディタ上で「UnityARKitPlugin/Examples/FaceTracking/FaceBlendShapeSloth」を開きます。
 * 「Console」のEditorから「Unknown: iPhone (pidなんたら」となっているデバイスを選ぶ。Lightningケーブルで接続されたiPhone Xが正常に認識されていると表示される。

結局のところ、このARKitRemoteデバッグアプリとUnityエディタはIP通信をしているだけのため、無線経由でも動くのですが、どうもカメラ映像を送信するところが負荷が高いらしく、Lightningケーブル直結を強く推奨します。
また、実はiTunesをインストールしたWindows上のUnityエディタでも動きます。

== AR要素をサボってかんたんにする

ARKit はAPIとしては色々なことができるのですが、3Dモデルに適用して遊ぶ際には「顔の向き・目・口」のみを扱うようにだけ絞ると、とても楽に扱うことができます。

=== 現実世界のカメラ画像は扱わない

@<code>{UnityARVideo}等のカメラまわりは使わないので適用しません。
Unity通常のカメラの正面にモデルを配置して、近距離でも歪まないようにFoVを30あたりの少し低めに設定します。

=== AR的な移動は使わない

サンプルでは鉛直方向に追従する設定になっている箇所を、フロントカメラの向き（≒iPhoneのディスプレイに正対する向き）のみ扱うように変更します。
これにより、現実世界の動きにカメラのプロジェクションをあわせる話を考えなくて済むようになります。

具体的にはARSessionまわり初期化コードのうち、@<code>{UnityARAlignment.UnityARAlignmentCamera}を設定します。
このパラメータの意味については「What does UnityARAlignment mean in Unity ARKit Plugins?　@<fn>{stackoverflow-unityaralignment}」とApple公式docをあわせて読むのがわかりやすいです。

//list[ARKitTracker1][ARKitTracker.cs ARKit初期化処理]{
m_session = UnityARSessionNativeInterface.GetARSessionNativeInterface();

Application.targetFrameRate = 60;
ARKitFaceTrackingConfiguration config = new ARKitFaceTrackingConfiguration();
// ARではなくてAvatarシステムとかだと非Gravityが楽
config.alignment = UnityARAlignment.UnityARAlignmentCamera;
config.enableLightEstimation = true;
//}

//footnote[stackoverflow-unityaralignment][@<href>{https://stackoverflow.com/questions/50716817/}]

=== 顔のMeshは使わない

「FaceBlendshapeScene」シーンにあるような、顔表面のリアルなマスクなようなMeshを取得するまわりも扱いません。

=== 顔の位置と角度はモデルのPositionと頭のRotationに適用

ARKitのトラッキング結果として取得できる顔の位置と角度は、それぞれモデルのPositionと頭のRotationに適用します。
iPhoneの正面に立って、キャラクターが鏡として動くように適当に符号や向きの入れ替えを行います。

//list[AvatarTracker1][AvatarTracker.cs 顔の位置と角度のトラッキング部]{
var p = new Vector3(
    _pose[ARKitPose.Index.PosY],
    -_pose[ARKitPose.Index.PosX],
    _pose[ARKitPose.Index.PosZ] * 1.5f);
var r = Quaternion.Euler(
    -_pose[ARKitPose.Index.RotY],
    -_pose[ARKitPose.Index.RotX],
    -_pose[ARKitPose.Index.RotZ] + 90.0f);

_poser.Update(p, r);
//}

以下のコードでは、角度を特定ボーンのみに適用すると首がかなりグキッといってしまうのを、頭から首、背骨の方へ4ボーンの指定があった際には分散するようにしています。

//list[Poser1][Poser.cs PositionとRotationの適用部]{
_targetRoot.position = pos;

if (_targetHeads.Length == 1)
{
    _targetHeads[0].localRotation = _defaultHeadsRot[0] * rot;
}
else if (_targetHeads.Length == 4)
{
    _targetHeads[0].localRotation =
        _defaultHeadsRot[0] * Quaternion.Lerp(_defaultHeadsRot[0], rot, 0.5f);
    _targetHeads[1].localRotation =
        _defaultHeadsRot[1] * Quaternion.Lerp(_defaultHeadsRot[1], rot, 0.35f);
    _targetHeads[2].localRotation =
        _defaultHeadsRot[2] * Quaternion.Lerp(_defaultHeadsRot[2], rot, 0.1f);
    _targetHeads[3].localRotation =
        _defaultHeadsRot[3] * Quaternion.Lerp(_defaultHeadsRot[3], rot, 0.05f);
}
//}

=== 50種以上とれる表情BlendShapeは一部だけ使う

前章で紹介した通り、表情BlendShapeとして取得できるパラメータは非常に多いのですが、一部だけ適用しましょう。
ミニマムでは「口を開く@<code>{jawOpen}」「左右の目のまばたき@<code>{eyeBlink_*}」と、「左右の目の上下左右@<code>{eyeLook}」をマッピングするぐらいでなんとかなります。

以下のコードでは加えて、「笑顔度」の@<code>{mouthSmile}を適用して、少しニンマリできるようにしています。

//list[AvatarTracker2][AvatarTracker.cs BlendShapeトラッキング適用部]{
if (_targetModel == Model.VRoid)
{
    // jawOpen
    _targetBlendShape.SetBlendShapeWeight(VRoidMouthJoy,
        _jawOpenCurve.Evaluate(_pose[ARKitPose.Index.JawOpen]) * 100.0f);
    // eyeWide not supported
    // eyeBlink
    _targetBlendShape.SetBlendShapeWeight(VRoidLeftBlinkJoy,
        _eyeBlinkCurve.Evaluate(_pose[ARKitPose.Index.EyeBlinkLeft]) * 100.0f);
    _targetBlendShape.SetBlendShapeWeight(VRoidRightBlinkJoy,
        _eyeBlinkCurve.Evaluate(_pose[ARKitPose.Index.EyeBlinkRight]) * 100.0f);
    // mouthSmile
    _targetBlendShape.SetBlendShapeWeight(VRoidMouthFun, mouthSmile * 100.0f);
}

// hidarime
if (_leftEyeRot)
{
    float lookInLeft = _pose[ARKitPose.Index.EyeLookInLeft];
    float lookOutLeft = _pose[ARKitPose.Index.EyeLookOutLeft];
    float lookUpLeft = _pose[ARKitPose.Index.EyeLookUpLeft];
    float lookDownLeft = _pose[ARKitPose.Index.EyeLookDownLeft];
    _leftEyeRot.transform.localEulerAngles = new Vector3(
        (lookDownLeft - lookUpLeft) * 10.0f,
        (lookInLeft - lookOutLeft) * 15.0f,
        0);
}

// migime
if (_rightEyeRot)
{
    float lookInRight = _pose[ARKitPose.Index.EyeLookInRight];
    float lookOutRight = _pose[ARKitPose.Index.EyeLookOutRight];
    float lookUpRight = _pose[ARKitPose.Index.EyeLookUpRight];
    float lookDownRight = _pose[ARKitPose.Index.EyeLookDownRight];
    _rightEyeRot.transform.localEulerAngles = new Vector3(
        (lookDownRight - lookUpRight) * 10.0f,
        (lookOutRight - lookInRight) * 15.0f,
        0);
}
//}

== VRoidでつくった3Dモデルを読み込む

ARKit Facial Trackingのトラッキング結果をどう扱うか決めたあとは、最終的に3Dモデルに適用します。

ここでは、VRMの仕様に対応するために表情BlendShapeが一通り揃ったVRoid Studioが出力するVRMを利用します。

本書ではVRoid Studio@<fn>{vroid-studio}はv0.2.11から出力したVRMを、UniVRM@<fn>{univrm}はv0.43を利用しています。
これらのバージョンについて、Unityエディタ上でimportする際には手作業で修正できるので問題ないのですが、normal mapのランタイムimportまわりが決着がついていない印象です。
前述のUnityやARKit Pluginがそれなりに古くとも問題ないのに対して、こちらの2つはリリース時期をあわせたバージョンで、なるべく新しいものを使うべきだと思われます。

//footnote[vroid-studio][VRoid Studio @<href>{https://vroid.pixiv.net/}]
//footnote[univrm][UniVRM @<href>{https://github.com/dwango/UniVRM}]



