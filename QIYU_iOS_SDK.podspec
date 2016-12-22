Pod::Spec.new do |s|

    s.name     = 'QIYU_iOS_SDK'
    s.version  = '3.1.2'
    s.license  = { :"type" => "Copyright",
                 :"text" => " Copyright 2016 Netease \n"}  
    s.summary  = '网易七鱼客服访客端 iOS SDK'
    s.homepage = 'http://www.qiyukf.com'
    s.author   = { 'qiyukf' => 'yunshangfu@126.com' }
    s.source   = { :git => 'https://github.com/qiyukf/QIYU_iOS_SDK.git', :tag => "3.1.2" }
    s.platform = :ios
    s.public_header_files = '**/ExportHeaders/**/*.h'
    s.source_files = "**/ExportHeaders/**/*.h"
    s.resource  = "**/QYResource.bundle"
    s.framework = 'UIKit','CoreText','MobileCoreServices','SystemConfiguration','AVFoundation','CoreTelephony','CoreMedia','AudioToolbox'
    s.libraries = 'z','stdc++.6.0.9','sqlite3.0'
    s.requires_arc = true


    s.subspec 'libQYSDK' do |ss|
        ss.vendored_libraries = '**/libQYSDK.a'
    end

    s.subspec 'libaacplus' do |ss|
        ss.vendored_libraries = '**/libaacplus.a'
    end

    s.subspec 'libcrypto' do |ss|
        ss.vendored_libraries = '**/libcrypto.a'
    end

    s.subspec 'libevent' do |ss|
        ss.vendored_libraries = '**/libevent.a'
    end

  s.default_subspecs = 'libQYSDK', 'libaacplus', 'libcrypto', 'libevent'

end
