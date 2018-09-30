#@# VRコンテンツのつくりかた4 技術書典5
#@# （）かっこは全角統一する。
#@# ユーザ入力は""ダブルクォーテーション、メニュー表示文字列は「」かぎかっこにでくくる。

= はじめに

就職しました（言い訳から入るスタイル）。

というわけで、「VRコンテンツのつくりかた4」と題しまして、「iPhone XのARKit Face Trackingで3Dモデル動かすとかんたんでおもしろい」という話を本書ではお届けします。
@<br>{}

本書は、以下のような方をターゲットにしています。

 * iPhone XのARKit Face Trackingが気になる人
 * Unityがちょっと分かって、X系のiPhoneを持っていてTrueDepthカメラで遊んでみたい人
 * 最近かわいい3Dモデルをホイホイ買ったはいいものの使い道がない人

== 本書の賞味期限とリポジトリ公開について

なお、本書は記載内容の賞味期限が切れてしまうことへのアップデート対応、あまり紙メディアが向いていない環境等のバージョンアップに追従するため、初版以降のPDFと関連ファイル一式を全て以下のリポジトリで公開予定です。
あらかじめご了承ください。

 * @<href>{https://github.com/youten/howto-create-vr-contents4}
 ** Re:VIEWの素材一式、出力PDF、関連プロジェクトのソースコードを全て含む想定です。
 * @<href>{https://github.com/youten/YXAvatar}
 ** 本書の後半で扱う、iPhone XのARKit Face TrackingでVRMキャラクターを動かすアバターアプリのUnityプロジェクト一式です。
 ** 他OSSをいくつか含みます、それぞれのライセンスは配下のドキュメントに従います。

本書のうち、私youtenが著作権を有する範囲のライセンスについては、文章はCC-BY 4.0ライセンス@<fn>{cc-by-4}を適用します。
ソースコードについてはMITライセンス@<fn>{mit-license}を適用します。

本書籍はRe:VIEWで作成されており、その設定ファイル等について、MITライセンスに基づき「C89 初めてのRe:VIEW v2」リポジトリ@<fn>{techbooster-c89-review-v2}で公開されているものを利用させていただいております。

//footnote[techbooster-c89-review-v2][@<href>{https://github.com/TechBooster/C89-FirstStepReVIEW-v2}]
//footnote[cc-by-4][@<href>{https://creativecommons.org/licenses/by/4.0/legalcode.ja}]
//footnote[mit-license][@<href>{https://opensource.org/licenses/MIT}]
