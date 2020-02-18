<?php

/**
*
* @file
* define a request for heimdal library login
*
*/

class HeimdalCurl extends MicroCURL {

    /**
    * Do a request for attributes and parse response into an array
    * @param string $bearer
    *   User specific access token from Heimdal
    * @param array $config
    *   Service configuration
    * @return string
    */
    public function getUserInfo($bearer, $config) {
        $this->set_url($config['heimdal_client_url'] . 'userinfo/');
        $this->set_option(CURLOPT_HTTPHEADER, array("Authorization: Bearer $bearer"));
        $attributes = $this->get();

        return $attributes;
    }

    /**
    * Get a link to heimdal access platform to retrive attributes associated
    * with given clientId and token
    * @param string $auth_code
    *   Authorization code from Heimdal
    * @param array $config
    *   Service configuration
    * @return string
    */
    public function getAccessToken($auth_code, $config) {
        $client_id = $config['heimdal_client_id'];
        $client_secret = $config['heimdal_client_secret'];

        $redirect_url = HeimdalHelpers::heimdal_get_full_url('heimdal_login');

        $token_post_data = "grant_type=authorization_code&code=$auth_code&client_id=$client_id&client_secret=$client_secret&redirect_uri=$redirect_url";
        $this->set_post($token_post_data);
        $this->set_url($config['heimdal_client_url'] . 'oauth/token');

        $this->set_option(CURLOPT_RETURNTRANSFER, TRUE);
        $this->set_option(CURLOPT_HTTPHEADER, array('Content-Type: application/x-www-form-urlencoded'));

        $response = $this->get();

        if($this->checkMe()) {
            return $response;
        }
        return '';
    }

    /**
    * Check status of given status array from parent class (@see MicroCURL::get_status()).
    * @param array $curlStatus
    * @return bool
    */
    public function checkMe() {
        $curlStatus = $this->get_status();
        // check for httpcode
        $flag = TRUE;
        if ($curlStatus['http_code'] !== 200) {
            watchdog('heimdal', 'url: @url returned: @code', array('@url' => $curlStatus['url'], '@code' => $curlStatus['http_code']), WATCHDOG_ERROR);
            $flag = FALSE;
        }
        // @TODO more checks

        return $flag;
    }

    /**
    * Overwrite parent::get .. mainly to do some logging
    *
    * @param bool $urls
    * @return string
    */
    public function get($urls = FALSE) {
        $response = parent::get($urls);
        // @TODO some logging
        return $response;
    }


}
