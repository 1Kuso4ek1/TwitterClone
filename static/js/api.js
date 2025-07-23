class WhoItterAPI {
    constructor() {
        this.baseURL = '/api';
    }

    async getCurrentUser() {
        const response = await fetch(`${this.baseURL}/users/me`);
        return response.json();
    }

    async createPost(content) {
        const response = await fetch(`${this.baseURL}/posts`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({ content })
        });
        return response.json();
    }

    async getFeed() {
        const response = await fetch(`${this.baseURL}/posts`);
        return response.json();
    }

    async getUserPosts(userId) {
        const response = await fetch(`${this.baseURL}/posts/user/${userId}`);
        return response.json();
    }

    async likePost(postId) {
        await fetch(`${this.baseURL}/like/${postId}`, {
            method: 'POST'
        });
    }

    async unlikePost(postId) {
        await fetch(`${this.baseURL}/like/${postId}`, {
            method: 'DELETE'
        });
    }

    async followUser(userId) {
        await fetch(`${this.baseURL}/follow/${userId}`, {
            method: 'POST'
        });
    }

    async unfollowUser(userId) {
        await fetch(`${this.baseURL}/follow/${userId}`, {
            method: 'DELETE'
        });
    }

    async logout() {
        await fetch('/logout', {
            method: 'POST'
        });
        window.location.href = '/login';
    }
}
