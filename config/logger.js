module.exports = {
	defaultLevel: 'ERROR',
	level: {
		market: 'INFO',
		trade: 'INFO',
		ctp: 'INFO',
		// 之所以打开以下category, 是为了兼容之前项目
		app: 'INFO',
		email: 'INFO',
		sql: 'INFO',
		websocket: 'INFO',
		schedule: 'INFO'
	}
};
