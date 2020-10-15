/*
 * Copyright (c) 2014 Lukasz Marek
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavformat/avio.h>
// 读取一个文件夹，并且把里面的东西全部输出出来，类似于 ls 命令
// 用法举例：输出当前目录
// ./avio_list_dir ./

// 输出结果：
// TYPE              SIZE                           NAME   UID(GID) UGO         MODIFIED         ACCESSED   STATUS_CHANGED
// <FILE>           38364                 filter_audio.o    502(20) 644 1602768574000000 1602768581000000 1602768574000000
// <FILE>        16045968                   encode_video    502(20) 755 1602768585000000 1602768585000000 1602768585000000
// <FILE>            8676                avio_list_dir.o    502(20) 644 1602768573000000 1602768579000000 1602768573000000
// <FILE>        18340216                    extract_mvs    502(20) 755 1602768585000000 1602768585000000 1602768585000000
// <FILE>           10002              vaapi_transcode.c    502(20) 644 1602767420000000 1602767624000000 1602767420000000
// <FILE>        17831720                 decode_audio_g    502(20) 755 1602768580000000 1602768584000000 1602768580000000
// <FILE>           34630                transcode_aac.c    502(20) 644 1602767420000000 1602768574000000 1602767420000000

// 这个是把 AVIODirEntry 里面的 int type 变成可读字符串，然后输出给人看
// 
static const char *type_string(int type)
{
    switch (type) {
    case AVIO_ENTRY_DIRECTORY:
        return "<DIR>";
    case AVIO_ENTRY_FILE:
        return "<FILE>";
    case AVIO_ENTRY_BLOCK_DEVICE:
        return "<BLOCK DEVICE>";
    case AVIO_ENTRY_CHARACTER_DEVICE:
        return "<CHARACTER DEVICE>";
    case AVIO_ENTRY_NAMED_PIPE:
        return "<PIPE>";
    case AVIO_ENTRY_SYMBOLIC_LINK:
        return "<LINK>";
    case AVIO_ENTRY_SOCKET:
        return "<SOCKET>";
    case AVIO_ENTRY_SERVER:
        return "<SERVER>";
    case AVIO_ENTRY_SHARE:
        return "<SHARE>";
    case AVIO_ENTRY_WORKGROUP:
        return "<WORKGROUP>";
    case AVIO_ENTRY_UNKNOWN:
    default:
        break;
    }
    return "<UNKNOWN>";
}

static int list_op(const char *input_dir)
{
    AVIODirEntry *entry = NULL;
    AVIODirContext *ctx = NULL;
    int cnt, ret;
    // cnt 是什么缩写？
    char filemode[4], uid_and_gid[20];
    // filemode 和 uid_and_gid 是做什么

    // 如果打开目录出错
    // 为什么要给个 context 过去？
    if ((ret = avio_open_dir(&ctx, input_dir, NULL)) < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot open directory: %s.\n", av_err2str(ret));
        goto fail;
    }
    // 给 ctx 的意思是读完了之后结果扔里头

    cnt = 0;
    for (;;) {
        // for 循环这么写是啥意思
        if ((ret = avio_read_dir(ctx, &entry)) < 0) {
            av_log(NULL, AV_LOG_ERROR, "Cannot list directory: %s.\n", av_err2str(ret));
            goto fail;
        }
        if (!entry)
            break;
        if (entry->filemode == -1) {
            snprintf(filemode, 4, "???");
            // snprintf 是干什么
        } else {
            snprintf(filemode, 4, "%3"PRIo64, entry->filemode);
        }
        snprintf(uid_and_gid, 20, "%"PRId64"(%"PRId64")", entry->user_id, entry->group_id);
        if (cnt == 0)
            av_log(NULL, AV_LOG_INFO, "%-9s %12s %30s %10s %s %16s %16s %16s\n",
                   "TYPE", "SIZE", "NAME", "UID(GID)", "UGO", "MODIFIED",
                   "ACCESSED", "STATUS_CHANGED");
            // 结果 cnt 的用途是让表头只输出一次？
        av_log(NULL, AV_LOG_INFO, "%-9s %12"PRId64" %30s %10s %s %16"PRId64" %16"PRId64" %16"PRId64"\n",
               type_string(entry->type),
               entry->size,
               entry->name,
               uid_and_gid, // uid 和 gid 是什么
               filemode, // 644, 755 之类的值
               entry->modification_timestamp, // 时间戳
               entry->access_timestamp,
               entry->status_change_timestamp);
            // AVIODirEntry 就是 entry
            // -> 箭头是啥意思
        avio_free_directory_entry(&entry); // 释放？
        cnt++;
    };

  fail:
    avio_close_dir(&ctx);
    return ret;
}

static void usage(const char *program_name)
{
    fprintf(stderr, "usage: %s input_dir\n"
            "API example program to show how to list files in directory "
            "accessed through AVIOContext.\n", program_name);
}

int main(int argc, char *argv[])
{
    int ret; // 存返回代码

    // log 设置等级
    av_log_set_level(AV_LOG_DEBUG);

    // 如果参数不足
    if (argc < 2) {
        usage(argv[0]);
        return 1;
    }

    avformat_network_init();
    // 这是干什么？查了说明这个好像已经没用了……

    ret = list_op(argv[1]);

    avformat_network_deinit();

    return ret < 0 ? 1 : 0; // 不是 0 的返回代码都通通返回 1 
}
