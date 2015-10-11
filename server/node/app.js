#!/usr/bin/env node
"use strict";

var program = require('commander'),
    querystring = require('querystring'),
    chalk = require('chalk'),
    https = require('https'),
    peristream = require('peristream');

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
 * Send heart command to the particle API
 * @param {Int} heartsCounts hearts count since we listen to the periscope
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


var stream = peristream(program.url);
var heartsCount = 0;

stream.connect().then(function(emitter){
    emitter.on(peristream.HEARTS, function(message){
        heartsCount++;
        if(Date.now() - lastPost >= DELTA_POST){
            lastPost = Date.now();
            console.log(action('Send heart from ' + message.displayName));
            sendHeart(heartsCount);
        }
  });

    emitter.on(peristream.DISCONNECT, function(message){
        console.log(info('Disconnected.'));
    });
});
