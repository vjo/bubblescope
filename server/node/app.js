#!/usr/bin/env node
"use strict";

var program = require('commander'),
    querystring = require('querystring'),
    chalk = require('chalk'),
    fs = require('fs'),
    https = require('https');

var PERISCOPE_URL_RE = /^https:\/\/www.periscope.tv\/w\/*/i,
    DELTA_POST = 400, // Sending post request every DELTA_POST ms
    CONFIG = require('./../../config.json'),
    lastPost = 0;

// define color helper for terminal output
var error = chalk.red,
    info = chalk.blue,
    action = chalk.magenta;

program
    .version('0.0.1')
    .option('-u, --url <url>', 'periscope URL', null)
    .parse(process.argv);

/*
 * Checking cli args
 */
if(!process.argv.slice(2).length) {
    program.help();
}
if(!program.url) {
    console.log(error('Error: you must specify a periscope URL.'));
    program.help();
}
if(!program.url.match(PERISCOPE_URL_RE)) {
    console.log(error('Error: wrong URL format'));
    program.help();
}

/**
 * Get PubNub key to subscribe
 * @param {String} token periscope token
 */
function getAccessPublic(token){
    var data = [];
    return https.get('https://api.periscope.tv/api/v2/getAccessPublic?broadcast_id=' + token, function(res){
        res.on('data', function (chunk){
            data.push(chunk);
        });
        res.on('end', function(){
            var d = JSON.parse(data.join(''));
            var keys = {
                auth_key      : d.auth_token,
                subscribe_key : d.subscriber,
                publish_key   : d.publisher,
                channel       : d.channel
            };
            subscribeOnPubnub(keys);
        });
    }).on('error', function(e) {
        console.log(error('Error getAccessPublic: ' + e.message));
    });
};

/**
 * Subscribe to PubNub API
 * @param {Object} data pubnub's subscription data
 */
function subscribeOnPubnub(data){
    var pubnub = require("pubnub")({
        ssl           : true,
        publish_key   : data.publish_key,
        subscribe_key : data.subscribe_key
    });
    var heartsCount = 0;
    pubnub.subscribe({
        channel    : data.channel,
        auth_key   : data.auth_key,
        callback   : function(message){
            if(message.type === 2){
                heartsCount++;
                if(Date.now() - lastPost >= DELTA_POST){
                    lastPost = Date.now();
                    console.log(action('Send heart from ' + message.displayName));
                    sendHeart(heartsCount);
                }
            }
        },
        connect    : function(){ console.log(info('Connected to PubNub!')); },
        disconnect : function(){ console.log(info('Disconnected from PubNub.')); },
        error      : function (error) {
            console.log(error('Error subscribe PubNub'));
            console.log(JSON.stringify(error));
        }
    });
};

/**
 * Send heart command to the particle API
 */
function sendHeart(heartsCount) {
    var post_data = querystring.stringify({"args": heartsCount});
    var post_options = {
        host: 'api.particle.io',
        port: '443',
        path: '/v1/devices/' + CONFIG.device_name + '/heart?access_token=' + CONFIG.access_token,
        method: 'POST',
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded',
            'Content-Length': post_data.length
        }
    };
    var post_req = https.request(post_options, function(res) {
        res.setEncoding('utf8');
        res.on('data', function (chunk) {
            if(chunk.error){
                console.log(error('Error post: ' + chunk.error));
            }
        });
    });
    post_req.write(post_data);
    post_req.end();
};

var extract = program.url.split('https://www.periscope.tv/w/')[1];
var token = encodeURIComponent(extract);
var data = getAccessPublic(token);
